#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9          
#define SS_PIN 10

byte old_uid[] = {0x00, 0x00, 0x00, 0x00};
byte old_bufferSize = 0;

MFRC522 mfrc522;
void setup() {
    Serial.begin(9600); 
    SPI.begin(); 
    mfrc522.PCD_Init(SS_PIN, RST_PIN);
    Serial.print(F("Reader "));
    Serial.print(F(": "));
    mfrc522.PCD_DumpVersionToSerial();

    Serial.println("\nPlease place the original card.");
}


void loop() {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        bool isSameCard = true;
        for(int i=0; i<mfrc522.uid.size; i++)
        {
            if(old_uid[i] != mfrc522.uid.uidByte[i])
            {
                isSameCard = false;
                break;
            }
        }
        if(old_bufferSize == 0 || isSameCard)
        {
            Serial.print("\nOriginal Card ID: ");
            for(int i=0; i<mfrc522.uid.size; i++)
                old_uid[i] = mfrc522.uid.uidByte[i];
            old_bufferSize = mfrc522.uid.size;
            dump_byte_array(old_uid, old_bufferSize);
            Serial.println();
        }
        else
        {
            Serial.print("\nNew Card Old ID: ");
            dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
            Serial.println();
            mfrc522.MIFARE_SetUid(old_uid, old_bufferSize, true);
            Serial.println("Copy Success.");
        }
        mfrc522.PICC_HaltA();
    }
}

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
