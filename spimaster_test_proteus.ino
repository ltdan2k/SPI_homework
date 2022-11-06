#define TFULL       50
#define THAlf       TFULL/2

#define SCK_PIN  4
#define MOSI_PIN 5
#define MISO_PIN 6
#define SS_PIN   7

#define SCK_OUTPUT          DDRD |= (1 << DDD4)
#define MOSI_OUTPUT         DDRD |= (1 << DDD5)
#define SS_OUTPUT           DDRD |= (1 << DDD7)
#define MISO_INPUT          DDRD &= ~(1 << DDD6)

#define write_MOSI(x)       PORTD =((x) ? (PORTD | 1<<PD5) :(PORTD & (~(1<<PD5))))
#define write_SS(x)         PORTD =((x) ? (PORTD | 1<<PD7) :(PORTD & (~(1<<PD7))))
#define write_SCK(x)        PORTD =((x) ? (PORTD | 1<<PD4) :(PORTD & (~(1<<PD4))))
#define read_MISO(x)        ( (PIND & (1 << PIND6)) ? HIGH : LOW)

void setup()
{
    SPI_setup();

}

void loop()
{
    uint8_t rev;
    SPI_begin();
    rev = SPI_transfer('a');
    SPI_end();
    delay(1000);
    Serial.println(rev);
    Serial.println("Master:" + String((char*)rev));

}

void SPI_setup()
{
    MOSI_OUTPUT;
    MISO_INPUT;
    write_SCK(LOW);
    SCK_OUTPUT;
    write_SS(HIGH);
    SS_OUTPUT;
    delay(1);

}

void SPI_begin(void)
{
    write_SS(LOW);
}

void SPI_end(void)
{
    write_SCK(LOW);
    write_SS(HIGH);
}

//write_MOSI(HIGH);
// ibit =0x80, ibit = ibit >>1;
uint8_t SPI_transfer(uint8_t byte_out)
{
    uint8_t byte_in =0;
    uint8_t ibit ,res;

    for(ibit=0x80; ibit >0;ibit = ibit>>1)
    {
        res = byte_out & ibit ;
        write_MOSI(res);

        delayMicroseconds(THAlf);
        write_SCK(HIGH);

        if(read_MISO() == HIGH)
        byte_in = byte_in | ibit;

        delayMicroseconds(THAlf);
        write_SCK(LOW);



    }


    return byte_in ;
}
