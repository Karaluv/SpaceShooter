#ifndef PLAYER_ACTIONS
#define PLAYER_ACTIONS

struct Player_Actions
{
	bool shout;
	long double* weapon_speed;
	unsigned hp;
	unsigned recharging_time;
	Player_Actions() : recharging_time(0), shout(false), hp(50)
	{
		weapon_speed = new long double[3]{ 0 };
	};
};
#endif