const int leftMotorPin = 6;
const int rightMotorPin = 5;

const int leftDetectorPin = 8;
const int rightDetectorPin = 7;

const int speedControlPin = 1;

enum Position
{
	Left,
	Center,
	Right
};

void move(bool left, bool right);

void applySpeedControl();

void setup()
{
	pinMode(leftMotorPin, OUTPUT);
	pinMode(rightMotorPin, OUTPUT);

	pinMode(leftDetectorPin, INPUT);
	pinMode(rightDetectorPin, INPUT);
}

int MOTOR_ON;
#define MOTOR_OFF 255

// HIGH = no obstacle = line
// LOW = obstacle = white
#define LINE_FOUND 0x1
#define WHITE_FOUND 0x0

bool lineLeft;
bool lineRight;

Position linePosition = Center;

void doNothing() {}

void loop()
{
	applySpeedControl();

	lineLeft = digitalRead(leftDetectorPin) == LINE_FOUND;
	lineRight = digitalRead(rightDetectorPin) == LINE_FOUND;

	//	doNothing();

	//	move(isObstacleLeft == LINE_FOUND, isObstacleRight == LINE_FOUND);
	//	move(false, false);

	//	return;

	if (!lineLeft && !lineRight)
	{
		switch (linePosition)
		{
		case Left:
			move(false, true);
			break;

		case Center:
			move(true, true);
			break;

		case Right:
			move(true, false);
			break;
		}
	}
	else if (lineLeft && !lineRight)
	{
		// Moving left
		move(false, true);
		linePosition = Left;
	}
	else if (!lineLeft && lineRight)
	{
		// Moving right
		move(true, false);
		linePosition = Right;
	}
	else
	{
		// Both sensors found the line
		move(true, true);
		linePosition = Center;
	}
}

void move(bool left, bool right)
{
	analogWrite(leftMotorPin, left ? MOTOR_ON : MOTOR_OFF);
	analogWrite(rightMotorPin, right ? MOTOR_ON : MOTOR_OFF);
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
}