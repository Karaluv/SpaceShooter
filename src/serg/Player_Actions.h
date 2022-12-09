#ifndef PLAYER_ACTIONS
#define PLAYER_ACTIONS

struct Player_Actions
{
	bool shout;
	long double* weapon_speed;
	unsigned hp;
	unsigned recharging_time;
	bool recharging;
	unsigned const full_recharging_time;
	Player_Actions() : full_recharging_time(100), recharging(false), recharging_time(0), shout(false), hp(50)
	{
		weapon_speed = new long double[3]{ 0 };
	};
};
#endif