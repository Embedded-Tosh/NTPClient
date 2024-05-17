#include "WiFiUdp.h"


// NTP Server
const char* ntpS = "pool.ntp.org";
const int ntpPort = 123;
const int localPort = 2390; // Local port to listen for UDP packets
// Add offset for IST (5 hours 30 minutes)
const int timeZoneOffset = 5 * 3600 + 30 * 60;// +5:30 India Time

  // NTP time is in the first 48 bytes of the message
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];

WiFiUDP udp;

// Function to send an NTP request packet
void sendNTPpacket(const char* address) {
  memset(packetBuffer, 0, NTP_PACKET_SIZE);

  // Initialize values needed to form NTP request
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;            // Stratum, or type of clock
  packetBuffer[2] = 6;            // Polling Interval
  packetBuffer[3] = 0xEC;         // Peer Clock Precision

  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;

  // Send packet to NTP server
  udp.beginPacket(address, ntpPort);
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}


// Function to get NTP time
unsigned long getNTPTime() {
  udp.begin(localPort); // Start listening on the local port

  sendNTPpacket(ntpS); // Send an NTP request

  // Wait for a response with a timeout
  unsigned long startMillis = millis();
  while (millis() - startMillis < 1000) {
    int size = udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      udp.read(packetBuffer, NTP_PACKET_SIZE);

      // Timestamp starts at byte 40
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
      unsigned long secsSince1900 = (highWord << 16) | lowWord;

      // Convert NTP time to Unix time (seconds since Jan 1 1970)
      const unsigned long seventyYears = 2208988800UL;
      unsigned long epoch = secsSince1900 - seventyYears;

      return epoch;
    }
  }
  return 0; // Return 0 if no packet was received
}

// Function to print local time with offset for IST (UTC+5:30)
bool GetLocalTime(struct tm* timeVale) {
  unsigned long epochTime = getNTPTime();
  if (epochTime == 0) {
    return false;
  }

  // Add offset for IST (5 hours 30 minutes)
  epochTime += timeZoneOffset;

  time_t rawtime = (time_t)epochTime;
  struct tm* tempTime = gmtime(&rawtime);
  *timeVale = *tempTime;
   return true;
}
