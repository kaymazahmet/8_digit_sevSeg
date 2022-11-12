

int buton= A0;
bool flg1 = 0;
bool flg2 = 0;
int c = 0;


static unsigned char bcd[] =
{
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

unsigned char _7seg[] =
{ //nGFEDCBA
  0b11000000,//sıfır/0
  0b11111001,//bir/1
  0b10100100,//iki/2
  0b10110000,//üç/3
  0b10011001,//dört/4
  //nGFEDCBA
  0b10010010,//beş/5
  0b10000010,//altı/6
  0b11111000,//yedi7
  0b10000000,//sekiz8
  0b10010000,//dokuz9
  0b00000000,//lamba TEST10
  0b11111111,//lamba OF11
};



bool snb = 0;
bool sno = 0;

bool dkb = 0;
bool dko = 0;

bool sab = 0;
bool sao = 0;

static void bcd_inc(unsigned char* bcd)
{
  if(c == 1){
  ++bcd[0];//her zaman kesmesinde 1 arttır
  // msb == 1;

  if (bcd[0] == 10)//10 kere zaman kesmesinde(100ms) olunca BİRLER BASAMAĞI
  {
    bcd[0] = 0;//1 LER BASAMAĞINI SIFIRLA
    ++bcd[1];//ONLAR BASAMAĞI 1 ARTTIR
    //mso == 1;

    if (bcd[1] == 10)//ONLAR BASAMAĞI 10 OLDUYSA 100x10 = 1000mS= 1 SANIYE
    {
      bcd[1] = 0; //10LAR BASAMAĞI SIFIRLAMA
      ++bcd[2]; // SANIYE BIRLER BASAMAĞI
      snb = 1;

      if (bcd[2] == 10)//SANIYE BIRLER BASAMAĞI 10 OLDUYSA
      {
        bcd[2] = 0;
        ++bcd[3];//SANİYE ONLAR BASAMAĞINI 1 ARTTIR
        sno = 1;

        if (bcd[3] == 6)//SANIYE 10LAR BASAMAĞI 6 OLDUYSA YANI 1 DAKIKA
        {
          bcd[3] = 0;
          ++bcd[4];//DAKIKA BİRLER BASAMAĞI 1 ARTTIR
          dkb = 1;
          if (bcd[4] == 10)//DAKIKA BIRLER BASAMAĞI 10 OLDUYSA
          {
            bcd[4] = 0;
            ++bcd[5];//DAKIKA ONLAR BASAMAĞI 1 ARTTIR
            dko = 1;
            if (bcd[5] == 6)//DAKIKA ONLAR BASAMAĞI 6 OLDUYSA YANİ 1 SAAT
            {
              bcd[5] = 0;
              ++bcd[6];//SAAT BIRLER BASAMAĞI 1 ARTTIR
              sab = 1;
              if ((bcd[6] == 4) && (bcd[7] == 2)) // EĞER SAAT 24 OLDUYSA
              {
                bcd[6] = bcd[7] = 0;//SAATI SIFIRLA
              }
              else if (bcd[6] == 10)
              {
                bcd[6] = 0;
                ++bcd[7];
                 sao = 1;
                 }
                }
              }
            }
         }
       }
     }
   }
 }


ISR(TIMER1_COMPA_vect) {
  bcd_inc(bcd);

    PORTD = _7seg[bcd[0]];
    PORTB = 0B00111110;
    delayMicroseconds(20);//tarama ayar;
    PORTB |= B00111111;

    PORTD = _7seg[bcd[1]];
    PORTB = 0B00111101;
    delayMicroseconds(20);//tarama ayar;
    PORTB |= B00111111;

    if (snb == 1) {
    PORTD = _7seg[bcd[2]];
    PORTB = 0B00111011;
    delayMicroseconds(20);//tarama ayar
    PORTB = B00111111;
    }

    if (sno == 1) {
    PORTD = _7seg[bcd[3]];
    PORTB = 0B00110111;
    delayMicroseconds(20);//tarama ayar;
    PORTB = B00111111;
    }

    if (dkb == 1) {
    PORTD = _7seg[bcd[4]];
    PORTB = B00101111;
    delayMicroseconds(50);//tarama ayar;
    PORTB = B00111111;
    }

    if (dko == 1) {
    PORTD = _7seg[bcd[5]];
    PORTB = B00011111;
    delayMicroseconds(50);//tarama ayar;
    PORTB = B00111111;
    }

  if(sab == 1){
  PORTD = _7seg[bcd[6]];
  PORTC = B100000; 
  delayMicroseconds(50);
  PORTC = B110000;
}

  if(sao == 1){
  PORTD = _7seg[bcd[7]];
  PORTC = B010000;
  delayMicroseconds(50);
  PORTC = B110000;
}
}



void setup()
{
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  DDRB  |= B00111111;
  PORTB |= B00111111;
  DDRC  |= B110000;
  PORTC |= B110000;

  DDRD = B11111111; //büTÜN D PORTI ÇIKIŞ
  DDRB = DDRB | B00001111;//8-9-10-11 çıkış

  noInterrupts();
  TCCR1A = 0;
  TCNT1 = 0;
  OCR1A = 16000000 / (100 * 64) - 1;
  TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

void loop()
{
    if(analogRead(buton) > 480 && analogRead(buton) < 600  && flg2 == 0){//sfırlama
    c = 15;
    flg2 = 1;
  }
    if(analogRead(buton) > 900 && flg1 == 1 ){    
    flg1 = 0;
  }
  
  if(analogRead(buton) > 170 && analogRead(buton) < 250 && flg1 == 0){//play-pause
    c++;
    flg1 = 1;
  }
  if(analogRead(buton) > 900 && flg2 == 1){
    flg2 = 0;
  }
    if(c == 15) {    
    bcd[0] = 0;
    bcd[1] = 0;
    bcd[2] = 0;
    bcd[3] = 0;
    bcd[4] = 0;
    bcd[5] = 0;
    bcd[6] = 0;
    bcd[7] = 0;
    bcd[8] = 0;
  }
  
  if(c > 1){
    delay(5);
    c = 0;
   }
  }


 
  


  
