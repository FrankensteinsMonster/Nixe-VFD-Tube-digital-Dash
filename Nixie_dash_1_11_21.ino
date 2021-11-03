/*
--__8_
d3|_2_| d7
d4|___| d6
---d5
//my really crappy 'art' showing which pin controls what part of the tube
//Would be better looking while i could do that ACSII art thing i guess?
//8,2&5 are the cross bars.
*/ 
//So this appears to be working out MPH to an ok-ish value. divide by 100? display code needs to be integrated now
const int grid_1 = A0;
const int grid_2 = A1;
const int grid_3 = A2;
const int grid_4 = A3;
const int grid_5 = A4;
const int vdf_8 = 8;
const int vdf_7 = 7;
const int vdf_6 = 6;
const int vdf_5 = 5;
const int vdf_4 = 4;
const int vdf_3 = 3;
const int vdf_2 = 2;
const int time_on = 2;
volatile unsigned long timestamp = 0;
volatile unsigned long timestamp2 = 0;
volatile unsigned long timestamp3 = 0;
volatile int showDigit = false;
volatile int gridNum = A0;
volatile int showDigOn = false;
volatile int wheelSensor = 0;
volatile int wheelSensorTrip = false;
volatile int lastWheelSensorTrip = false;
int w = 0;
int x = 0;
int y = 0;
int z = 0;
int v = 0;

int sensorNods = 10;                               //How many sensor triggers are on the wheel (E.G. 10 raised spots around the hub for hall effect to find)
int wheelDiameter = 28;                            //How big in diameter, in inches, the inflated tire is bearing light load
int interNod = 0;                                  //Trying to calcuate the inches of wheel between each sensor trigger
int wheelInchs = 0;                                //the circumfrnce, rolling diameter 
int interMPH = 0;                                  //How many miles per hour are covered if the wheel is doing one rotation per second (eg 14 inch wheel x pi=43.982 divided by 17.6=2.4989.  Or 28 inchXpi=87.96=4.99)
int interInch = 0;                                 //i can't remember what this was.  Hope i don't need it?
volatile int sensorMilSecs = 0;
volatile int estSpeed = 0;
volatile int estWheelRPM = 0;
volatile int sensorMilSecWhole = 0;


void setup() {
  Serial.begin(250000);
  // put your setup code here, to run once:
  pinMode(grid_1, OUTPUT);
  pinMode(grid_2, OUTPUT);
  pinMode(grid_3, OUTPUT);
  pinMode(grid_4, OUTPUT);
  pinMode(grid_5, OUTPUT);
  pinMode(vdf_8, OUTPUT);
  pinMode(vdf_7, OUTPUT);
  pinMode(vdf_6, OUTPUT);
  pinMode(vdf_5, OUTPUT);
  pinMode(vdf_4, OUTPUT);
  pinMode(vdf_3, OUTPUT);
  pinMode(vdf_2, OUTPUT);
  pinMode(A7, INPUT_PULLUP);

  wheelDiameter *= 100;                         //arduino sucks at math.  doesn't like decimals.  Increase by 10-100 or 1000 first. Then divide
  wheelInchs = (wheelDiameter * 3.1415);
  Serial.print(" circumf = ");
  Serial.print(wheelInchs);
  interMPH = (wheelInchs / 176);
  Serial.print(" interMPH = ");
  Serial.print(interMPH);
  interNod = (wheelInchs / sensorNods);
  Serial.print(" interNod measured = ");
  Serial.print(interNod);
  interInch = (wheelInchs * sensorNods);               //This isn't right....
  Serial.print(" interInch = ");
  Serial.print(interInch);
  estSpeed = (interMPH * (sensorMilSecs / sensorNods));    //No it's not.  That's a mistake
  estSpeed = 0;                                         //A test of start screen
}

void loop() {
  // put your main code here, to run repeatedly:

  if (analogRead(A7) >= 500)
    {
      wheelSensorTrip = true;
    }
  else if (analogRead(A7) <= 500)
    {
      wheelSensorTrip = false;
    }
    
  if ((lastWheelSensorTrip != wheelSensorTrip))
    {
      if (wheelSensorTrip == true)
      {
        timestamp3 = timestamp2;
        timestamp2 = millis();
        //Serial.print(" Wheel Sensor triggered at- ");
        //Serial.print(timestamp2);
        timestampCompare();
        lastWheelSensorTrip = wheelSensorTrip;
      }
      else
      {
        lastWheelSensorTrip = wheelSensorTrip;
      }
    }
  // print estSpeed to tubes here
  w = (int) (estSpeed / 1000);
  x = (int) (estSpeed / 100 );
  x = (int) (x - (w*10));
  y = (int) (estSpeed / 10);
  y = (int) (y - (w * 100));
  y = (int) (y - (x * 10));
  z = (int) (estSpeed - (w * 1000));
  z = (int) (z - (x * 100));
  z = (int) (z - (y * 10));

  //Serial.print(" Showing first digit  ");
  showW();
  allOff();
  //Serial.print(" Showing second digit  ");
  showX();
  allOff();
  //Serial.print(" Showing third digit  ");
  showY();
  allOff();
  //Serial.print(" Showing fourth digit  ");
  showZ();
  allOff();
}

void timestampCompare()
{
  sensorMilSecs = (timestamp2 - timestamp3);

  sensorMilSecWhole = (sensorMilSecs * sensorNods);  //how many milliseconds would one wheel rotation make

  estWheelRPM = (10000 / sensorMilSecWhole);           //i thought, in my sick state, that 1,000ms is one second, divide 1 second by the milisec' for whole rotate, give how many rotat per second. why it always say 0?

  //sensorMilSecs /=100;
  estSpeed = (estWheelRPM * interMPH);
}
//end math segment here

void timeDigit()
{
 if (showDigit == true)
 {
  timestamp = millis();
  showDigOn = true;
 }
  if (showDigOn == true)
  {
    while ((timestamp + time_on) >= millis());
    allOff();
  }
}

void numberOne()
{
  digitalWrite((gridNum), HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  showDigit = true;
}

void numberTwo()
{
  digitalWrite((gridNum), HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(2, HIGH);
  showDigit = true;
}

void numberThree()
{
  digitalWrite((gridNum), HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(2, HIGH);
  showDigit = true;
}

void numberFour()
{
  digitalWrite((gridNum), HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  showDigit = true;
}

void numberFive()
{
  digitalWrite((gridNum), HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(2, HIGH);
  showDigit = true;
}

void numberSix()
{
  digitalWrite((gridNum), HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(4, HIGH);
  showDigit = true;
}

void numberSeven()
{
  digitalWrite((gridNum), HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  showDigit = true;
}

void numberEight()
{
  digitalWrite((gridNum), HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(2, HIGH);
  showDigit = true;
}

void numberNine()
{
  digitalWrite((gridNum), HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(2, HIGH);
  showDigit = true;
}

void numberZero()
{
  digitalWrite((gridNum), HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(3, HIGH);
  showDigit = true;
}

void allOff()
{
  digitalWrite(grid_1, LOW);
  digitalWrite(grid_2, LOW);
  digitalWrite(grid_3, LOW);
  digitalWrite(grid_4, LOW);
  digitalWrite(grid_5, LOW);
  digitalWrite(vdf_8, LOW);
  digitalWrite(vdf_7, LOW);
  digitalWrite(vdf_6, LOW);
  digitalWrite(vdf_5, LOW);
  digitalWrite(vdf_4, LOW);
  digitalWrite(vdf_3, LOW);
  digitalWrite(vdf_2, LOW);
  //delay(1);  //to prevent "ghost" effect to other tube
  delayMicroseconds(100);
  showDigit = false;
  showDigOn = false;
}

void showW()
{
  v = w;
  gridNum = grid_5;
  if (showDigit == false)
  {
    digi2Output();
    timeDigit();
  }
}

void showX()
{
  v = x;
  gridNum = grid_4;
  if (showDigit == false)
  {
    digi2Output();
    timeDigit();
  }
}

void showY()
{
  v = y;
  gridNum = grid_2;
  if (showDigit == false)
  {
    digi2Output();
    timeDigit();
  }
}

void showZ()
{
  v = z;
  gridNum = grid_1;
  if (showDigit == false)
  {
    digi2Output();
    timeDigit();
  }
}

void digi2Output()
{
  if (v == 0)
  {
    numberZero();
  }
  if (v == 1)
  {
    numberOne();
  }
  if (v == 2)
  {
    numberTwo();
  }
  if (v == 3)
  {
    numberThree();
  }
  if (v == 4)
  {
    numberFour();
  }
  if (v == 5)
  {
    numberFive();
  }
  if (v == 6)
  {
    numberSix();
  }
  if (v == 7)
  {
    numberSeven();
  }
  if (v == 8)
  {
    numberEight();
  }
  if (v == 9)
  {
    numberNine();
  }
}
