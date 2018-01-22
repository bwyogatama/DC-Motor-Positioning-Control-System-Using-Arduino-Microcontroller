//Pin Setup
int pin_pwm = 5;
int adc1 = A5;
int adc2 = A4;
int obv = A3;

//Variable declaration
int v_in_int = 0;
int v_out_int = 0;
float v_in_f;
float v_out_f;
float error;
float error_delay = 0.0;
float error_delay2 = 0.0;
float pwm_f;
float pwm_delay = 0.0;
float pwm_delay2 = 0.0;
float pwm_f_t;
int pwm_int;
int mod_sig;

void setup() 
{
  pinMode(adc1, INPUT); //assign pin A5 as INPUT
  pinMode(adc2, INPUT); //assign pin A4 as INPUT
  pinMode(pin_pwm, OUTPUT); //assign pin 5 as OUTPUT
  Serial.begin(9600); //Start your serial port
}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}


void loop() 
{ 
  setPwmFrequency(5, 1);
  
  /** Analog to Digital Converter Section **/
  v_in_int = analogRead(adc1);
  v_out_int = analogRead(adc2);

  v_in_f = v_in_int*5.0/1023.0;
  v_out_f = v_out_int*5.0/1023.0;

  
  error = v_in_f - v_out_f;

  /** Controller Section **/
  //Using PD
  pwm_f = -pwm_delay + 137.8965*error - 76.6475*error_delay;

  //without controller
  //pwm_f = error;
  

  error_delay2 = error_delay;
  error_delay = error;
  pwm_delay2 = pwm_delay;
  pwm_delay = pwm_f;

  /** Pulse Width Modulation Section **/
  pwm_f_t = (pwm_f + 5.0)/2.0;

  pwm_int = (int)((pwm_f_t * 255.0)/5.0);

  if (pwm_int <=255 & pwm_int >0)
    analogWrite(pin_pwm, pwm_int);
  else
  {
    if (pwm_int>255)
      analogWrite(pin_pwm,255);
    else
      analogWrite(pin_pwm,0);
  }

  /** Sampling Period **/
  delay(100);

  mod_sig = analogRead(obv);
  /** Monitoring Variable using Serial Monitor **/
  Serial.print(v_out_f); Serial.print(",");
  Serial.println(v_in_f);
  
}

