#ifndef PLAYER_ACTIONS
#define PLAYER_ACTIONS

using Type = long double;
struct Player_Actions
{
	bool shout;
	Type* weapon_speed;
	unsigned hp;
	unsigned recharging_time;
	Player_Actions() : recharging_time(0), shout(false), hp(50)
	{
		weapon_speed = new Type[3]{ 0 };
	};
};
#endif