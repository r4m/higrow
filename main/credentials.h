#define WIRELESS_MODE_INFRA	1  // Infrastructure (based on Access Point)
#define WIRELESS_MODE_ADHOC	2  // Ad-hoc (no Access Point)

unsigned char local_ip[] = {10, 1, 38, 10};      // IP address
unsigned char gateway_ip[] = {10, 1, 38, 254};	    // Gateway IP address
unsigned char subnet_mask[] = {255, 255, 254, 0};   // Subnet Mask
const prog_char ssid[] PROGMEM = {"GUEST"};	    // SSID access point

unsigned char security_type = 3;  // 0 -> open
                                  // 1 -> WEP
                                  // 2 -> WPA
                                  // 3 -> WPA2

// WPA/WPA2 password (max 64 char)
const prog_char security_passphrase[] PROGMEM = {"foobar"};

// WEP 64/128 bit keys password
prog_uchar wep_keys[] PROGMEM = {0x12, 0x34, 0x56, 0x78, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

unsigned char wireless_mode = WIRELESS_MODE_INFRA;

unsigned char ssid_len;
unsigned char security_passphrase_len;






