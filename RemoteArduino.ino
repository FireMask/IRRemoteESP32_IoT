#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>

//SSID/Pass Red WiFi
const char* ssid = "SSID";
const char* password = "Wifi-Password";

const uint16_t kRecvPin = 27;     // Pin del receptor infrarrojo
const uint16_t IR_PIN   = 26;     // Pin del emisor infrarrojo
const bool EnableIRRead = true;  //Cambiar a true para habilitar la lecturade codigos infrarojos

WebServer server(80);
IRsend irsend(IR_PIN);
IRrecv irrecv(kRecvPin);
decode_results results; //

void handleRoot() {
  //Inicializar la pagina principal
  String html = "<script>";
    html += "    document.write(unescape('%3C%21DOCTYPE%20html%3E%0A%3Chtml%3E%0A%0A%3Chead%3E%0A%20%20%20%20%3Ctitle%3EControl%20Remoto%20de%20TV%3C/title%3E%0A%20%20%20%20%3Cmeta%20name%3D%22viewport%22%20content%3D%22width%3Ddevice-width%2C%20initial-scale%3D1.0%2C%20user-scalable%3Dno%22%3E%0A%20%20%20%20%3Cstyle%3E%0A%20%20%20%20%20%20%20%20html%2C%0A%20%20%20%20%20%20%20%20body%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20-webkit-user-select%3A%20none%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20/*%20Safari%20*/%0A%20%20%20%20%20%20%20%20%20%20%20%20-moz-user-select%3A%20none%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20/*%20Firefox%20*/%0A%20%20%20%20%20%20%20%20%20%20%20%20-ms-user-select%3A%20none%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20/*%20IE%2010+/Edge%20*/%0A%20%20%20%20%20%20%20%20%20%20%20%20user-select%3A%20none%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20/*%20Est%E1ndar%20*/%0A%20%20%20%20%20%20%20%20%7D%0A%0A%20%20%20%20%20%20%20%20.remote-control%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20margin%3A%2010px%200%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20padding%3A%200%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20width%3A%20100%25%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20background-color%3A%20rgb%28235%2C%20181%2C%20115%29%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20border-radius%3A%2010px%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20display%3A%20flex%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20flex-wrap%3A%20wrap%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20justify-content%3A%20space-between%3B%0A%20%20%20%20%20%20%20%20%7D%0A%0A%20%20%20%20%20%20%20%20.control-name%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20width%3A%20100%25%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20text-align%3A%20center%3B%0A%20%20%20%20%20%20%20%20%7D%0A%0A%20%20%20%20%20%20%20%20.button%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20width%3A%20calc%28100%25%20/%205%29%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20height%3A%20100px%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20margin%3A%205px%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20font-size%3A%2024px%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20font-weight%3A%20bold%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20text-align%3A%20center%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20text-decoration%3A%20none%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20border-radius%3A%204px%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20background-color%3A%20%23525252%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20color%3A%20%23fff%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20cursor%3A%20pointer%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20display%3A%20flex%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20justify-content%3A%20center%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20align-items%3A%20center%3B%0A%20%20%20%20%20%20%20%20%7D%0A%0A%20%20%20%20%20%20%20%20.red%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20background-color%3A%20%23c33838%3B%0A%20%20%20%20%20%20%20%20%7D%0A%0A%20%20%20%20%20%20%20%20.blue%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20background-color%3A%20%233b53c9%3B%0A%20%20%20%20%20%20%20%20%7D%0A%0A%20%20%20%20%20%20%20%20.green%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20background-color%3A%20%23478123%3B%0A%20%20%20%20%20%20%20%20%7D%0A%0A%20%20%20%20%20%20%20%20.magenta%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20background-color%3A%20%237a1088%3B%0A%20%20%20%20%20%20%20%20%7D%0A%0A%20%20%20%20%20%20%20%20.yellow%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20color%3A%20black%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20background-color%3A%20%23f5ff3b%3B%0A%20%20%20%20%20%20%20%20%7D%0A%0A%20%20%20%20%20%20%20%20@media%20screen%20and%20%28max-width%3A%20480px%29%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20.button%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20padding%3A%208px%2016px%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20font-size%3A%2014px%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20%7D%0A%20%20%20%20%20%20%20%20%7D%0A%20%20%20%20%3C/style%3E%0A%3C/head%3E%0A%0A%3Cbody%3E%0A%20%20%20%20%3Cdiv%20class%3D%22remote-control%22%3E%0A%20%20%20%20%20%20%20%20%3Ch1%20class%3D%22control-name%22%3EQuick%20Actions%3C/h1%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20class%3D%22button%20green%22%20onclick%3D%22quickActionButton%28%27turnOnOffAll%27%29%22%3ETurn%20On/Off%20All%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20class%3D%22button%20yellow%22%20onclick%3D%22quickActionButton%28%27changeInput%27%29%22%3EChange%20Input%3C/div%3E%0A%20%20%20%20%3C/div%3E%0A%0A%20%20%20%20%3Cdiv%20class%3D%22remote-control%22%3E%0A%20%20%20%20%20%20%20%20%3Ch1%20class%3D%22control-name%22%3ETV%3C/h1%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%221%22%20class%3D%22button%20red%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x20df10ef%27%29%22%3EPower%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%222%22%20class%3D%22button%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x20dfc23d%27%29%22%3EConfig%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%223%22%20class%3D%22button%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x20dfd02f%27%29%22%3EInput%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%224%22%20class%3D%22button%20blue%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x20df40bf%27%29%22%3EVol%20+%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%225%22%20class%3D%22button%20green%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x20df02fd%27%29%22%3EUp%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%226%22%20class%3D%22button%20blue%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x20df00ff%27%29%22%3ECh%20%5E%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%227%22%20class%3D%22button%20green%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x20dfe01f%27%29%22%3ELeft%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%228%22%20class%3D%22button%20magenta%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x20df22dd%27%29%22%3EOk%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%229%22%20class%3D%22button%20green%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x20df609f%27%29%22%3ERight%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2210%22%20class%3D%22button%20blue%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x20df807f%27%29%22%3ECh%20v%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2211%22%20class%3D%22button%20green%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x20df827d%27%29%22%3EDown%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2212%22%20class%3D%22button%20blue%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x20dfc03f%27%29%22%3EVol%20-%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2213%22%20class%3D%22button%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x20df3ec1%27%29%22%3EHome%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2214%22%20class%3D%22button%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x20df906f%27%29%22%3EMute%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2215%22%20class%3D%22button%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x20dfda25%27%29%22%3EExit%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2216%22%20class%3D%22button%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x20df14eb%27%29%22%3EBack%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2217%22%20class%3D%22button%22%20onclick%3D%22actionButton%28%27%27%2C%20%27%27%29%22%3E%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2218%22%20class%3D%22button%22%20onclick%3D%22actionButton%28%27%27%2C%20%27%27%29%22%3E%3C/div%3E%0A%20%20%20%20%3C/div%3E%0A%0A%20%20%20%20%3Cdiv%20class%3D%22remote-control%22%3E%0A%20%20%20%20%20%20%20%20%3Ch1%20class%3D%22control-name%22%3EHome%20Theater%3C/h1%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2219%22%20class%3D%22button%20red%22%20onclick%3D%22actionButton%28%27SONY%27%2C%20%270x540c%27%29%22%3EPower%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2220%22%20class%3D%22button%20blue%22%20onclick%3D%22actionButton%28%27SONY%27%2C%20%270x640c%27%29%22%3EVol%20-%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2221%22%20class%3D%22button%20blue%22%20onclick%3D%22actionButton%28%27SONY%27%2C%20%270x240c%27%29%22%3EVol%20+%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2222%22%20class%3D%22button%20green%22%20onclick%3D%22actionButton%28%27SONY%27%2C%20%270x4b0d%27%29%22%3EInput%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2223%22%20class%3D%22button%20green%22%20onclick%3D%22actionButton%28%27SONY%27%2C%20%270x140c%27%29%22%3EMute%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2224%22%20class%3D%22button%22%20onclick%3D%22actionButton%28%27%27%2C%20%27%27%29%22%3E%3C/div%3E%0A%20%20%20%20%3C/div%3E%0A%0A%20%20%20%20%3Cdiv%20class%3D%22remote-control%22%3E%0A%20%20%20%20%20%20%20%20%3Ch1%20class%3D%22control-name%22%3EHDMI%20Switch%3C/h1%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2225%22%20class%3D%22button%20red%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x44bbe01f%27%29%22%3EPower%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2226%22%20class%3D%22button%20blue%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x44bbf00f%27%29%22%3E1%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2227%22%20class%3D%22button%20blue%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x44bb48b7%27%29%22%3E2%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2228%22%20class%3D%22button%20blue%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x44bbe817%27%29%22%3E3%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2229%22%20class%3D%22button%20blue%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x44bb6897%27%29%22%3E4%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2230%22%20class%3D%22button%20magenta%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x44bbf807%27%29%22%3EArc%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2231%22%20class%3D%22button%20magenta%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x44bb2ad5%27%29%22%3E2.0%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2232%22%20class%3D%22button%20magenta%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x44bbf20d%27%29%22%3E5.1%3C/div%3E%0A%20%20%20%20%20%20%20%20%3Cdiv%20id%3D%2233%22%20class%3D%22button%20magenta%22%20onclick%3D%22actionButton%28%27NEC%27%2C%20%270x44bbd22d%27%29%22%3EPass%3C/div%3E%0A%20%20%20%20%3C/div%3E%0A%0A%20%20%20%20%3Cscript%3E%0A%0A%20%20%20%20%20%20%20%20function%20actionButton%28platform%2C%20code%29%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20postAsync%28platform%2C%20code%29%0A%20%20%20%20%20%20%20%20%7D%0A%0A%20%20%20%20%20%20%20%20function%20quickActionButton%28action%29%20%7B%0A%0A%20%20%20%20%20%20%20%20%20%20%20%20let%20timeSpan%20%3D%20100%0A%20%20%20%20%20%20%20%20%20%20%20%20let%20buttons%20%3D%20%5B%5D%0A%0A%20%20%20%20%20%20%20%20%20%20%20%20switch%20%28action%29%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20case%20%22turnOnOffAll%22%3A%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20buttons%20%3D%20%5B%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20document.getElementById%28%221%22%29%2C%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20document.getElementById%28%2219%22%29%2C%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20document.getElementById%28%2225%22%29%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%5D%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20break%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20case%20%22changeInput%22%3A%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20buttons%20%3D%20%5B%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20document.getElementById%28%223%22%29%2C%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20document.getElementById%28%2211%22%29%2C%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20document.getElementById%28%2211%22%29%2C%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20document.getElementById%28%228%22%29%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%5D%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20timeSpan%20%3D%202500%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20break%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20%7D%0A%0A%20%20%20%20%20%20%20%20%20%20%20%20let%20timesRun%20%3D%20buttons.length%0A%20%20%20%20%20%20%20%20%20%20%20%20if%28timesRun%29%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20let%20runTimes%20%3D%200%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20let%20interval%20%3D%20setInterval%28%28%29%20%3D%3E%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20console.log%28%27run%27%29%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20buttons%5BrunTimes++%5D.click%28%29%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20if%20%28runTimes%20%3E%3D%20timesRun%29%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20clearInterval%28interval%29%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%7D%2C%20timeSpan%29%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20%7D%0A%20%20%20%20%20%20%20%20%7D%0A%0A%20%20%20%20%20%20%20%20function%20postAsync%28platform%2C%20data%29%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20var%20formData%20%3D%20new%20FormData%28%29%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20formData.append%28%22platform%22%2C%20platform%29%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20formData.append%28%22code%22%2C%20data%29%3B%0A%0A%20%20%20%20%20%20%20%20%20%20%20%20fetch%28%27/send%27%2C%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20method%3A%20%22POST%22%2C%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20body%3A%20formData%0A%20%20%20%20%20%20%20%20%20%20%20%20%7D%29%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20.catch%28function%20%28error%29%20%7B%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20console.log%28error%29%3B%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%7D%29%3B%0A%20%20%20%20%20%20%20%20%7D%0A%20%20%20%20%3C/script%3E%0A%3C/body%3E%0A%0A%3C/html%3E'));";
    html += "</script>";
  server.send(200, "text/html", html);
}

void handleSend() {
  String platform = server.arg("platform");
  String code = server.arg("code");

  if (platform.length() > 0 && code.length() > 0) {
    sendCode(platform, code);
    Serial.println("Code sent: " + code);
    // handleRoot();
  } else {
    Serial.println("La plataforma o el codigo no se proporcionaron");
  }
}

void sendCode(String platform, String code){

  // https://github.com/crankyoldgit/IRremoteESP8266/blob/fdf35b49a8deba01dc6509fedb36cbb2cb5fe96a/src/IRsend.h
  // https://github.com/crankyoldgit/IRremoteESP8266/blob/1f79fee3b6c30e069dd616d03f5d1e6d6bd64a19/src/IRremoteESP8266.h
  unsigned long data = strtoul(code.c_str(), NULL, 16);

  if(platform == "NEC"){
    irsend.sendNEC(data);
  }
  else if(platform == "SONY"){
    irsend.sendSony(data, kSony15Bits);
  }
  else if(platform == "RC5"){
    irsend.sendRC5(data);
  }
  else if(platform == "RC6"){
    irsend.sendRC6(data);
  }
  else if(platform == "DISH"){
    irsend.sendDISH(data);
  }
  else if(platform == "SHARP"){
    // irsend.sendSharp(data);
  }
  else if(platform == "DENON"){
    irsend.sendDenon(data);
  }
  else if(platform == "MITSUBISHI"){
    irsend.sendMitsubishi(data);
  }
  else if(platform == "SAMSUNG"){
    irsend.sendSAMSUNG(data);
  }
  else if(platform == "LG"){
    irsend.sendLG(data);
  }
  else if(platform == "WHYNTER"){
    irsend.sendWhynter(data);
  }
  else if(platform == "AIWA_RC_T501"){
    irsend.sendAiwaRCT501(data);
  }
  else if(platform == "LG2"){
    irsend.sendLG2(data);
  }else{
    // irsend.sendGeneric(/*args*/])
    Serial.println("No se envio codigo porque la plataforma no esta en el listado");
  }
}

void setup() {
  Serial.begin(115200);

  // Conexión WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Iniciar el web server
  server.on("/", handleRoot);
  server.on("/send", handleSend);
  server.begin();


  if(EnableIRRead){
    // Iniciar el emisor infrarrojo
    irsend.begin();
    Serial.println("Emisor infrarrojo iniciado");

    irrecv.enableIRIn();  // Start the receiver
    while (!Serial)  // Wait for the serial connection to be establised.
      delay(50);
    Serial.println();
    Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
    Serial.println(kRecvPin);
  }
}

String GetProtocol(decode_type_t type){
  switch(type){
    case NEC:
      return "NEC";
    case SONY:
      return "SONY";
    case RC5:
      return "RC5";
    case RC6:
      return "RC6";
    case DISH:
      return "DISH";
    case SHARP:
      return "SHARP";
    case DENON:
      return "DENON";
    case MITSUBISHI:
      return "MITSUBISHI";
    case SAMSUNG:
      return "SAMSUNG";
    case LG:
      return "LG";
    case WHYNTER:
      return "WHYNTER";
    case AIWA_RC_T501:
      return "AIWA_RC_T501";
    case LG2:
      return "LG2";
    default:
      return "";
  }
}

void loop() {
  server.handleClient();

  if(EnableIRRead){
    if (irrecv.decode(&results)) {

      String protocolName = GetProtocol(results.decode_type);

      if (results.decode_type == NEC) {
        Serial.print("Código NEC: 0x");
        Serial.println(String(results.value, HEX));
      }else{
        Serial.print("Código " + protocolName + ": 0x");
        Serial.println(String(results.value, HEX));
      }
      irrecv.resume();  // Receive the next value
    }
    delay(100);
  }
}
