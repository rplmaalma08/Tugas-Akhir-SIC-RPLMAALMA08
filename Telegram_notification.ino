#include <WiFi.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

const char* ssid = " LAB RPL ";
const char* password = "maalmajaya";
const char* apiToken = "7387443358:AAFuKiqDTssnWN8MzYpvvmpV-2TkqNwpTKI";
const char* chatID = "7120240241";
const char* apiURL = "api.telegram.org";

WiFiClientSecure client;
DynamicJsonDocument jsonDoc(2048);

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println("Initializing Telegram bot...");
}

void sendToTelegram(String message) {
  String url = "/bot" + String(apiToken) + "/sendMessage";
  jsonDoc.clear();
  jsonDoc["chat_id"] = chatID;
  jsonDoc["text"] = message;

  String jsonString;
  serializeJson(jsonDoc, jsonString);

  client.setInsecure(); // Use setInsecure to bypass certificate verification
  client.connect(apiURL, 443);
  client.println("POST " + url + " HTTP/1.1");
  client.println("Host: " + String(apiURL));
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonString.length());
  client.println();
  client.println(jsonString);

  int timeout = 10000;
  while (!client.available() && (timeout-- > 0)) {
    delay(1);
  }

  if (!client.available()) {
    Serial.println("No response from Telegram");
  } else {
    char response[256];
    client.readBytes(response, 256);
    Serial.println(response);
  }
  client.stop(); // Close the connection
}

void loop() {
  sendToTelegram("Halo, remaja putri yang kami cintai,

Kami ingin mengingatkan Anda tentang pentingnya konsumsi tablet tambah darah yang mungkin sudah sedikit terlupakan minggu ini. Mengingat manfaat besar dari tablet ini untuk kesehatan Anda dan generasi anda nantinya, kami mohon agar Anda segera mengakses dan mengkonsumsi tablet tersebut.

Kenapa Ini Penting?

Tablet tambah darah sangat berperan dalam mencegah anemia dan memastikan tubuh Anda mendapatkan cukup zat besi. Jika Anda tidak rutin mengkonsumsinya, ada risiko serius yang dapat mempengaruhi kesehatan Anda dan masa depan Anda, seperti:

Stunting: Kekurangan zat besi dapat menyebabkan stunting, yaitu kondisi di mana pertumbuhan fisik dan perkembangan kognitif Anda dan anak yang akan anda lahirkan nanti terhambat. Stunting bisa menghambat kemajuan SDM dan mempengaruhi kualitas hidup generasi Anda di masa depan.
Kesehatan Umum: Anemia dapat menyebabkan kelelahan, penurunan konsentrasi, dan daya tahan tubuh yang lemah.
Tips untuk Konsumsi Tablet Tambah Darah Bagi yang Sulit Menelan Pil:

Campurkan dengan Makanan: Anda bisa menghancurkan tablet dan mencampurkannya dengan sedikit makanan lembut seperti yogurt atau bubur. Pastikan Anda memakannya segera setelah dicampurkan.

Gunakan Alat Pembantu: Jika tablet terlalu besar, coba gunakan alat penghancur tablet yang tersedia di apotek untuk memudahkan Anda.

Konsultasikan dengan Petugas Kesehatan: Jika Anda benar-benar kesulitan, bicarakan dengan dokter atau petugas kesehatan Anda untuk alternatif atau solusi lain yang bisa lebih mudah bagi Anda.

Jangan lupa, konsumsi tablet tambah darah ini hanya satu langkah kecil menuju kesehatan yang lebih baik. Kami percaya Anda bisa melakukannya dengan konsisten demi masa depan yang lebih sehat dan cerah.

Terima kasih atas perhatian Anda, dan jaga kesehatan dengan baik!

Salam sehat,");
  delay(10000);
}
