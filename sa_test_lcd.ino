/*******************************************************
 *                                                     * 
 *  SWITCHABLE ANTENNA                                 *
 *                                                     *
 *  (c) 2011 - Marco Letizia, Davide Letizia           * 
 *                                                     *
 *  <> Started at November 2011                        * 
 *                                                     *  
 *  VERSION: 0.8 - TEST                                      *
 *                                                     *
 *  LICENSE: GPL v.3                                   *
 *                                                     *
 *  contacts:  marco.letizia.ita@gmail.com             *
 *             davide.letizia@gmail.com                *
 *                                                     *
 *******************************************************/
 
// ARDUINO PINS
/*int PIN_ANT_IN_a = 2; 
int PIN_ANT_IN_b = 3;
int PIN_ANT_IN_c = 4;
int PIN_SIGN_IN = A0;*/
int PIN_SIGN_IN_0 = A0;
int PIN_SIGN_IN_1 = A1;
int PIN_SIGN_IN_2 = A2;
int PIN_SIGN_IN_3 = A3;

// VARIABLES
int tmpVal[8][10] = {{0,0,0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0,0,0},
                     {0,0,0,0,0,0,0,0,0,0}};
float avgVal[8] = {0,0,0,0,0,0,0,0};
int tmpsign;
float phi; // azimut

float VCC = 1.1; // AnalogReference(INTERNAL) ==> 1.1 Volt
float BestVoltage;
int pos_BestVoltage;
int pos_BestNear;
float BestAngle;
int pos_BestAngle;
float V_diff_between_near;
float residuo[23];

// TABLES
/*
 ___________________________
| GRADI [°] | GUADAGNO [dB] |
|___________|_______________|
|    ...    |     ...       |
|___________|_______________|
*/
float tableDiff[23][2] = {
                          { 0, 15.6578},
                          { 1, 15.0493},
                          { 2, 14.4099},
                          { 3, 13.7461},
                          { 4, 13.0638},
                          { 5, 12.3680},
                          { 6, 11.6628},
                          { 7, 10.9514},
                          { 8, 10.2363},
                          { 9,  9.5197},
                          {10,  8.8029},
                          {11,  8.0869},
                          {12,  7.3727},
                          {13,  6.6605},
                          {14,  5.9507},
                          {15,  5.2434},
                          {16,  4.5385},
                          {17,  3.8360},
                          {18,  3.1355},
                          {19,  2.4368},
                          {20,  1.7395},
                          {21,  1.0432},
                          {22,  0.3476}
                         };

// FUNCTIONS

void getBestAntenna (float *array, int dim){
  //int dim = size(array);
  //float pippo[2] = {0, 0};
  int tmpMax = 0;
  int index = 0;
  // Find Max Value
  for (int i=0; i<dim; i++){
    tmpMax = max(array[i], tmpMax);
  }
  // Find Index of Max Value
  for (int i=0; i<dim; i++) {
    if(array[i] == tmpMax)
        index = i;
  }
 // Return Values
 BestVoltage = tmpMax;
 pos_BestVoltage = index;
}

void getBestAngle (float *array, int dim){
  int tmpMin = 1023;
  int index = 0;
  // Find Max Value
  for (int i=0; i<dim; i++){
    tmpMin = min(array[i], tmpMin);
  }
  // Find Index of Min Value
  for (int i=0; i<dim; i++) {
    if(array[i] == tmpMin)
        index = i;
  }
 // Return Values
 BestAngle = tmpMin;
 pos_BestAngle = index;
}

int findBestNear (float *array, int idx_ant) {
  switch (idx_ant) {
    case 0:
      if (array[7] > array[1]) {
        tmpsign = -1;
        return 7;
      } else {
        tmpsign = +1;
        return 1;
      }
      break;
    case 7:
      if (array[6] > array[0]) {
        tmpsign = -1;
        return 6;
      } else {
        tmpsign = +1;
        return 0;
      }
      break;
    default:
      if (array[idx_ant-1] > array[idx_ant+1]) {
        tmpsign = -1;
        return idx_ant-1;
      } else {
        tmpsign = +1;
        return idx_ant+1;
      }
      break;  
  }
}

void getResiduo (float V_diff_dB) {
  for (int i=0; i<23; i++) {
    residuo[i] = abs(tableDiff[i][1] - V_diff_dB);
  }
}
/*
void selectAntenna_IN_Port(int i) {
  // xxx
  switch (i) {
    case 0:
      digitalWrite(PIN_ANT_IN_a, LOW);
      digitalWrite(PIN_ANT_IN_b, LOW);
      digitalWrite(PIN_ANT_IN_c, LOW);
      break;
    case 1:
      digitalWrite(PIN_ANT_IN_a, HIGH);
      digitalWrite(PIN_ANT_IN_b, LOW);
      digitalWrite(PIN_ANT_IN_c, LOW);
      break;
    case 2:
      digitalWrite(PIN_ANT_IN_a, LOW);
      digitalWrite(PIN_ANT_IN_b, HIGH);
      digitalWrite(PIN_ANT_IN_c, LOW);
      break;
   case 3:
      digitalWrite(PIN_ANT_IN_a, LOW);
      digitalWrite(PIN_ANT_IN_b, HIGH);
      digitalWrite(PIN_ANT_IN_c, HIGH);
      break;
   case 4:
      digitalWrite(PIN_ANT_IN_a, HIGH);
      digitalWrite(PIN_ANT_IN_b, LOW);
      digitalWrite(PIN_ANT_IN_c, LOW);
      break;
   case 5:
      digitalWrite(PIN_ANT_IN_a, HIGH);
      digitalWrite(PIN_ANT_IN_b, LOW);
      digitalWrite(PIN_ANT_IN_c, HIGH);
      break;
   case 6:
      digitalWrite(PIN_ANT_IN_a, HIGH);
      digitalWrite(PIN_ANT_IN_b, HIGH);
      digitalWrite(PIN_ANT_IN_c, LOW);
      break;
   case 7:
      digitalWrite(PIN_ANT_IN_a, HIGH);
      digitalWrite(PIN_ANT_IN_b, HIGH);
      digitalWrite(PIN_ANT_IN_c, HIGH);
      break;
  } 
}*/
// END FUNCTIONS

void setup() {
  // DEBUG
  Serial.begin(9600);
  
  analogReference(INTERNAL);
 /* 
  // SET PIN AS OUTPUT
  pinMode(PIN_ANT_IN_a, OUTPUT);
  pinMode(PIN_ANT_IN_b, OUTPUT);
  pinMode(PIN_ANT_IN_c, OUTPUT);*/
}

void loop() {
  // SCAN
  /*for (int r=0; r<10; r++) {
    for (int c=0; c<8; c++) {
      selectAntenna_IN_Port(c);
      delay(10);
      //Serial.print("PORTA NUM. ");
      //Serial.print(c);
      //Serial.println("  =  ");
      tmpVal[r][c] = analogRead(PIN_SIGN_IN);
      //Serial.println(tmpVal[r][c]);
      delay(100);
    }
  }
  // DONE SCAN
*/
 // SCAN
  for (int r=0; r<10; r++) {
    for (int c=0; c<4; c++) {
      //selectAntenna_IN_Port(c);
      //delay(10);
      //Serial.print("PORTA NUM. ");
      //Serial.print(c);
      //Serial.println("  =  ");
      if (c == 0) {
        tmpVal[r][c] = analogRead(PIN_SIGN_IN_0);
      }
      //Serial.println(tmpVal[r][c]);
      delay(100);
      if (c == 1) {
        tmpVal[r][c] = analogRead(PIN_SIGN_IN_1);
      }
      //Serial.println(tmpVal[r][c]);
      delay(100);
      if (c == 2) {
        tmpVal[r][c] = analogRead(PIN_SIGN_IN_2);
      }
      //Serial.println(tmpVal[r][c]);
      delay(100);
      if (c == 3) {
        tmpVal[r][c] = analogRead(PIN_SIGN_IN_3);
      }
      //Serial.println(tmpVal[r][c]);
      delay(100);
    }
    delay(100);
  }
  // DONE SCAN

  // MEAN
  for (int r=0; r<8; r++) {
    for (int c=0; c<10; c++) {
      avgVal[r] += tmpVal[r][c];
    }
    avgVal[r] /= 10; // valori da 0 a 1023
    
    avgVal[r] = avgVal[r] * VCC / 1024; // Valori in Volt da 0 a Vcc
  }
  // DONE MEAN 
 
 // MAX
 getBestAntenna(avgVal, 8); // Return to "BestVoltage"
 // DONE MAX
 
 // NEAR
 pos_BestNear = findBestNear(avgVal, pos_BestVoltage);
 // DONE NEAR
 
 // RESIDUO
 V_diff_between_near = 20 * log10(BestVoltage / avgVal[pos_BestNear]);
 getResiduo(V_diff_between_near);
 getBestAngle(residuo, 23);
 // DONE RESIDUO
 
 // AZIMUT
 phi = (45 * pos_BestVoltage) + (tmpsign * BestAngle); // attento ai numeri negativi -45:360
 // END AZIMUT
 
 // PRINT
 Serial.println("Num PORTA: ");
 Serial.println(pos_BestVoltage);

 Serial.println("Phi: ");
 Serial.println(phi);

}
