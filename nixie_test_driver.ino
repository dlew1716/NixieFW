#define ZERO 2
#define ONE 0
#define TWO 12
#define THREE 4
#define FOUR 8
#define FIVE 9
#define SIX 1
#define SEVEN 3
#define EIGHT 11
#define NINE 10

uint8_t lowerMapHHMM [] = {2, 0,12, 4, 8, 9, 1, 3,11,10,5};
uint8_t lowerMapss []   = {8, 9, 3,11,10, 2, 1,12, 4, 0,5};
uint8_t lowerMapmm []   = {4,12, 9,13, 5, 1, 8, 6, 2, 0,10};
uint8_t lowerMapSS []   = {1, 0, 6, 2, 4,12, 8, 9,13, 5,10};



void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(3, LOW); 
  Serial.begin(9600);
}

void setDigitPair(uint8_t num)
{
    
    for(int i = 0; i<8; i++)
    {
      digitalWrite(2, LOW); 
      digitalWrite(4, (num >> i) & 0x1);  
      digitalWrite(2, HIGH); 
    } 
    

}

void setNumber(uint32_t num)
{
  uint8_t HH,hh,MM,mm,SS,ss;
  //num <= 999999 ?  : num = 999999;
  HH = num / 100000;
  num = num - (uint32_t)HH*100000;
  hh = num / 10000;
  num = num - (uint32_t)hh*10000;
  MM = num / 1000;
  num = num - (uint32_t)MM*1000;
  mm = num / 100;
  num = num - (uint32_t)mm*100;
  SS = num / 10;
  num = num - (uint32_t)SS*10;
  ss = num ;

  setDigitPair(lowerMapHHMM[HH] | (lowerMapss[hh] << 4)); //HH hh
  setDigitPair(lowerMapHHMM[MM] | (lowerMapSS[SS] << 4)); //MM SS
  setDigitPair(lowerMapss[ss]   | (lowerMapmm[mm] << 4)); //ss mm
}

void digitize(uint32_t num, uint8_t *HH, uint8_t *hh, uint8_t *MM, uint8_t *mm, uint8_t *SS, uint8_t *ss)
{
  *HH = num / 100000;
  num = num - (uint32_t)*HH*100000;
  *hh = num / 10000;
  num = num - (uint32_t)*hh*10000;
  *MM = num / 1000;
  num = num - (uint32_t)*MM*1000;
  *mm = num / 100;
  num = num - (uint32_t)*mm*100;
  *SS = num / 10;
  num = num - (uint32_t)*SS*10;
  ss = num ;
}

void digitizeArr(uint32_t num, uint8_t *arr)
{
  arr[0] = num / 100000;
  num = num - (uint32_t)arr[0]*100000;
  arr[1] = num / 10000;
  num = num - (uint32_t)arr[1]*10000;
  arr[2] = num / 1000;
  num = num - (uint32_t)arr[2]*1000;
  arr[3] = num / 100;
  num = num - (uint32_t)arr[3]*100;
  arr[4] = num / 10;
  num = num - (uint32_t)arr[4]*10;
  arr[5] = num ;
}

void setFaceArr(uint8_t *aa)
{
  setDigitPair(lowerMapHHMM[aa[0]] | (lowerMapss[aa[1]] << 4)); //HH hh
  setDigitPair(lowerMapHHMM[aa[2]] | (lowerMapSS[aa[4]] << 4)); //MM SS
  setDigitPair(lowerMapss[aa[5]]   | (lowerMapmm[aa[3]] << 4)); //ss mm
  digitalWrite(3, HIGH);
  digitalWrite(3, LOW);
}

void setFace(uint8_t HH, uint8_t hh, uint8_t MM, uint8_t mm, uint8_t SS, uint8_t ss)
{
   
  setDigitPair(lowerMapHHMM[HH] | (lowerMapss[hh] << 4)); //HH hh
  setDigitPair(lowerMapHHMM[MM] | (lowerMapSS[SS] << 4)); //MM SS
  setDigitPair(lowerMapss[ss]   | (lowerMapmm[mm] << 4)); //ss mm
  digitalWrite(3, HIGH);
  digitalWrite(3, LOW);
  
}

void spinMode1(uint32_t num)
{
  uint8_t digitArrNext[6];
  uint8_t digitArr[6];
  int8_t digitCurrPos[6];

  digitizeArr(num,digitArrNext);

  for(int j = 0; j < 6; j++)
  {
    digitArr[j] = 0;
    digitCurrPos[j] = -1;
  }

  int j = -1;
  int m = 5;
  int lsbFirst = 5;
  int allDigitInPos = 0;

  while(!allDigitInPos)
  {
    j = (j+1)%10;
    digitArr[0] = j;
    setFaceArr(digitArr);
    delay(150);
    if(j == digitArrNext[lsbFirst])
    {
      digitCurrPos[lsbFirst] = 0;
      lsbFirst--;
    }

    
    for(int p = 5; p >= 0; p--)
    {

      if(digitCurrPos[p] >= 0 && digitCurrPos[p] < p)
      {
        digitArr[digitCurrPos[p]+1] = digitArr[digitCurrPos[p]];
        digitArr[digitCurrPos[p]] = 0;
        digitCurrPos[p]++;
      }
      else if(!digitCurrPos[0])
      {
        allDigitInPos = 1;
      }      
      
    }
    
    
  }
}

void spinMode2(uint32_t num)
{
  uint8_t digitArrNext[6];
  uint8_t digitArr[6];

  for(int j = 0; j < 6; j++)
  {
    digitArr[j] = 0;
  }
  
  digitizeArr(num,digitArrNext);

  int digRemain = 1;
  while(digRemain)
  {
    digRemain = 0;
    for(int i = 0; i < 6; i++)
    {
      if(digitArr[i] == digitArrNext[i])
      {
      }
      else
      {
        digRemain = 1;
        digitArr[i] = random(0,10);
      }
      
      
    }
    setFaceArr(digitArr);
    delay(50);
  }

}

void spinMode3(uint32_t num)
{
  uint8_t digitArrNext[6];
  uint8_t digitArr[6];
  int8_t digitCurrPos[6];

  digitizeArr(num,digitArrNext);

  for(int j = 0; j < 6; j++)
  {
    digitArr[j] = 10;
    digitCurrPos[j] = -1;
  }

  int j = -1;
  int m = 5;
  int lsbFirst = 5;
  int allDigitInPos = 0;

  while(!allDigitInPos)
  {
    j = (j+1)%10;
    digitArr[0] = j;
    setFaceArr(digitArr);
    delay(50);
    if(j == digitArrNext[lsbFirst])
    {
      digitCurrPos[lsbFirst] = 0;
      lsbFirst--;
    }

    
    for(int p = 5; p >= 0; p--)
    {

      if(digitCurrPos[p] >= 0 && digitCurrPos[p] < p)
      {
        digitArr[digitCurrPos[p]+1] = digitArr[digitCurrPos[p]];
        digitArr[digitCurrPos[p]] = 10;
        digitCurrPos[p]++;
      }
      else if(!digitCurrPos[0])
      {
        allDigitInPos = 1;
      }      
      
    }
    
    
  }
}

void shiftRight(uint8_t* arr)
{
  for (int i = 0; i < 5; i++)
  {
    arr[i+1] = arr[i];
  }
}



void loop() {
uint32_t k = 123456;
for(uint32_t i = 0; i <999999; i++)
{
  //spinMode1(123456);
  //setFaceArr(h);
  //delay(4000);
  
  spinMode3(random(0,999999));
  delay(2000);


}


}
