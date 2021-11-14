const int BMS_MIN_VOLTAGE = 10000; // mVolts
const int BMS_MAX_VOLTAGE = 12000;
const int BMS_MIN_CURRENT = 100; // mAmps
const int BMS_MAX_CURRENT = 3000;
const int BMS_MIN_TEMP = 15; // Celcius
const int BMS_MAX_TEMP = 40;

enum nodes {
	BMS,
	NAV,
	BRAKES,
	MOTOR,
	OBC
};

struct bms {
	nodes node = BMS;
	int voltage;
	int current;
	int temp;
};

bool get_bounded_random_number(int lower, int upper, int* n);
void update_bms(struct bms & _bms);
int stringify_bms(uint8_t* buffer, struct bms &_bms);
void seed_random(int seed);