const int LED = 13;

const int leftMotorPin = 5;
const int rightMotorPin = 6;

const int leftDetectorPin = 7;
const int rightDetectorPin = 8;

const int speedControlPin = 1;
const int delayControlPin = 2;

void move(bool left, bool right);

int fromPotToPercent(float pot);

int fromPercentToMotor(int percent);

void applySpeedControl();

void applyDelayControl();

enum Position
{
	Left,
	Center,
	Right
};

Position linePosition = Center;

void setup()
{
	pinMode(LED, OUTPUT);

	pinMode(leftMotorPin, OUTPUT);
	pinMode(rightMotorPin, OUTPUT);

	pinMode(leftDetectorPin, INPUT);
	pinMode(rightDetectorPin, INPUT);

	pinMode(speedControlPin, INPUT);
	pinMode(delayControlPin, INPUT);

//	Serial.begin(9600);
}

// HIGH = no obstacle = line
// LOW = obstacle = white
int isObstacleLeft;
int isObstacleRight;

int MOTOR_ON;
const int MOTOR_OFF = 255;

void loop()
{
	applySpeedControl();
	applyDelayControl();

	isObstacleLeft = digitalRead(leftDetectorPin);
	isObstacleRight = digitalRead(rightDetectorPin);

	if (isObstacleLeft == LOW && isObstacleRight == LOW)
	{
		switch (linePosition)
		{
		case Left:
			move(true, false);
			break;

		case Center:
			move(true, true);
			break;

		case Right:
			move(false, true);
			break;
		}
	}
	else if (isObstacleLeft == HIGH && isObstacleRight == LOW)
	{
//		Serial.print("Moving left");
		move(true, false);

		linePosition = Right;
	}
	else if (isObstacleRight == HIGH && isObstacleLeft == LOW)
	{
//		Serial.print("Moving right");
		move(false, true);

		linePosition = Left;
	}
	else
	{
//		Serial.print("Both sensors found the line");
		move(true, true);

		linePosition = Center;
	}

//	Serial.println();
}

int moveDelay;

unsigned long prevTime = 0;
unsigned long time;

int leftMotorValue;
int rightMotorValue;

void move(bool left, bool right)
{
	if (left && right)
	{
		time = millis();

//		Serial.print(" Time: ");
//		Serial.print(time);
//		Serial.print(". Prev time: ");
//		Serial.print(prevTime);

		if (time - prevTime < moveDelay)
		{
//			Serial.print(". Skipping due to delay");
			return;
		}

		prevTime = time;
	}

	leftMotorValue = left ? MOTOR_ON : MOTOR_OFF;
	rightMotorValue = right ? MOTOR_ON : MOTOR_OFF;

	analogWrite(leftMotorPin, leftMotorValue);
	analogWrite(rightMotorPin, rightMotorValue);
}

int fromPotToPercent(float pot)
{
	return pot / 1023 * 100;
}

int fromPercentToMotor(int percent)
{
	return 255 * percent / 100;
}

void applySpeedControl()
{
	MOTOR_ON = fromPercentToMotor(fromPotToPercent(analogRead(speedControlPin)));

//	Serial.print("Speed: ");
//	Serial.print(MOTOR_ON);
//	Serial.print(". ");
}

void applyDelayControl()
{
	moveDelay = 100 + fromPotToPercent(analogRead(delayControlPin));

//	Serial.print("Delay: ");
//	Serial.print(moveDelay);
//	Serial.print(". ");
}
