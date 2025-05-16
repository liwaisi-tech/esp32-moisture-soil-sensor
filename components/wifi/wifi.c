#include <string.h>
#include <sys/socket.h>  // Para AF_INET
#include <netinet/in.h>  // Para estructuras de direcciones IP
#include "wifi.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_mac.h"
#include "lwip/sockets.h"  // Para inet_pton
#include "lwip/dns.h"      // Para configuración DNS
#include "esp_netif_types.h"
#include "esp_netif_defaults.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1
#define WIFI_MAXIMUM_RETRY CONFIG_ESP_MAXIMUM_RETRY

/* Configuración de IP estática */
#ifdef CONFIG_STATIC_IP_ENABLED
    #define USE_STATIC_IP        CONFIG_STATIC_IP_ENABLED
    #define STATIC_IP_ADDR       CONFIG_STATIC_IP_ADDR
    #define STATIC_NETMASK       CONFIG_STATIC_NETMASK
    #define STATIC_GATEWAY       CONFIG_STATIC_GATEWAY
    #define STATIC_PRIMARY_DNS   CONFIG_STATIC_PRIMARY_DNS
    #define STATIC_SECONDARY_DNS CONFIG_STATIC_SECONDARY_DNS
#else
    #define USE_STATIC_IP        1
    #define STATIC_IP_ADDR       "192.168.1.100"
    #define STATIC_NETMASK       "255.255.255.0"
    #define STATIC_GATEWAY       "192.168.1.1"
    #define STATIC_PRIMARY_DNS   "8.8.8.8"
    #define STATIC_SECONDARY_DNS "8.8.4.4"
#endif

static const char *TAG = "wifi_station";
static EventGroupHandle_t s_wifi_event_group;
static int s_retry_num = 0;
static esp_netif_t *s_sta_netif = NULL;

static void event_handler(void* arg, esp_event_base_t event_base,
                         int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < WIFI_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "Reintentando conexión al AP (%d/%d)", s_retry_num, WIFI_MAXIMUM_RETRY);
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG, "Falló la conexión al AP");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "IP obtenida:" IPSTR, IP2STR(&event->ip_info.ip));
        ESP_LOGI(TAG, "Máscara de red:" IPSTR, IP2STR(&event->ip_info.netmask));
        ESP_LOGI(TAG, "Gateway:" IPSTR, IP2STR(&event->ip_info.gw));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

esp_err_t configure_static_ip(esp_netif_t *netif)
{
    if (netif == NULL) {
        ESP_LOGE(TAG, "Interfaz de red no válida");
        return ESP_FAIL;
    }

    /* Desactivar el cliente DHCP para IP estática */
    ESP_ERROR_CHECK(esp_netif_dhcpc_stop(netif));
    ESP_LOGI(TAG, "Cliente DHCP desactivado para configuración estática");

    /* Define la configuración de IP estática */
    esp_netif_ip_info_t ip_info;
    memset(&ip_info, 0, sizeof(esp_netif_ip_info_t));
    
    /* Método alternativo: Convierte las direcciones IP de texto a binario usando ip4addr_aton */
    if (!ip4addr_aton(STATIC_IP_ADDR, (ip4_addr_t*)&ip_info.ip.addr)) {
        ESP_LOGE(TAG, "Dirección IP inválida: %s", STATIC_IP_ADDR);
        return ESP_FAIL;
    }
    
    if (!ip4addr_aton(STATIC_NETMASK, (ip4_addr_t*)&ip_info.netmask.addr)) {
        ESP_LOGE(TAG, "Máscara de red inválida: %s", STATIC_NETMASK);
        return ESP_FAIL;
    }
    
    if (!ip4addr_aton(STATIC_GATEWAY, (ip4_addr_t*)&ip_info.gw.addr)) {
        ESP_LOGE(TAG, "Gateway inválido: %s", STATIC_GATEWAY);
        return ESP_FAIL;
    }

    /* Aplica la configuración de IP estática */
    esp_err_t ret = esp_netif_set_ip_info(netif, &ip_info);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Falló la configuración de IP estática: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ESP_LOGI(TAG, "Configurada IP estática: %s", STATIC_IP_ADDR);
    ESP_LOGI(TAG, "Configurada máscara de red: %s", STATIC_NETMASK);
    ESP_LOGI(TAG, "Configurado gateway: %s", STATIC_GATEWAY);
    
    /* Configura servidores DNS */
    esp_netif_dns_info_t dns_info;
    memset(&dns_info, 0, sizeof(esp_netif_dns_info_t));
    
    /* DNS Primario */
    if (ip4addr_aton(STATIC_PRIMARY_DNS, (ip4_addr_t*)&dns_info.ip.u_addr.ip4)) {
        esp_netif_set_dns_info(netif, ESP_NETIF_DNS_MAIN, &dns_info);
        ESP_LOGI(TAG, "DNS primario configurado: %s", STATIC_PRIMARY_DNS);
    }
    
    /* DNS Secundario */
    if (ip4addr_aton(STATIC_SECONDARY_DNS, (ip4_addr_t*)&dns_info.ip.u_addr.ip4)) {
        esp_netif_set_dns_info(netif, ESP_NETIF_DNS_BACKUP, &dns_info);
        ESP_LOGI(TAG, "DNS secundario configurado: %s", STATIC_SECONDARY_DNS);
    }
    
    return ESP_OK;
}

esp_err_t wifi_init_sta(void)
{
    esp_err_t ret = ESP_OK;
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    /* Crea la interfaz de red WiFi para modo estación */
    s_sta_netif = esp_netif_create_default_wifi_sta();
    if (!s_sta_netif) {
        ESP_LOGE(TAG, "Error: No se pudo crear la interfaz de red WiFi STA");
        return ESP_FAIL;
    }

    /* Configura IP estática si está habilitada */
    if (USE_STATIC_IP) {
        ret = configure_static_ip(s_sta_netif);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Error al configurar IP estática");
            return ret;
        }
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                      ESP_EVENT_ANY_ID,
                                                      &event_handler,
                                                      NULL,
                                                      &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                      IP_EVENT_STA_GOT_IP,
                                                      &event_handler,
                                                      NULL,
                                                      &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = CONFIG_ESP_WIFI_SSID,
            .password = CONFIG_ESP_WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Iniciando conexión WiFi a SSID: %s", CONFIG_ESP_WIFI_SSID);

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Conectado al AP SSID: %s", CONFIG_ESP_WIFI_SSID);
        
        /* Verificar que la IP estática se aplicó correctamente */
        if (USE_STATIC_IP) {
            esp_netif_ip_info_t ip_info;
            esp_netif_get_ip_info(s_sta_netif, &ip_info);
            ESP_LOGI(TAG, "Configuración IP actual:" IPSTR, IP2STR(&ip_info.ip));
        }
        
        ret = ESP_OK;
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGE(TAG, "Falló la conexión al AP SSID: %s, password: %s",
                 CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
        ret = ESP_FAIL;
    } else {
        ESP_LOGE(TAG, "Error inesperado");
        ret = ESP_ERR_INVALID_STATE;
    }

    return ret;
}

void get_mac_address(char* mac_str) {
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    snprintf(mac_str, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/* Función para obtener la dirección IP actual como string */
void get_ip_address(char* ip_str) {
    if (s_sta_netif == NULL) {
        strcpy(ip_str, "0.0.0.0");
        return;
    }
    
    esp_netif_ip_info_t ip_info;
    esp_netif_get_ip_info(s_sta_netif, &ip_info);
    snprintf(ip_str, 16, IPSTR, IP2STR(&ip_info.ip));
}