#include <iostream>
#include <net.h>

enum class CustomMsgTypes : uint32_t
{
	CastSpell,
	MovePlayer
};

class CustomClient : public peanut::net::client_interface <CustomMsgTypes>
{
public:
	bool CastSpell(float x, float y) {
		peanut::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::CastSpell;
		msg << x << y;
		Send(msg);
	}
};

int main()
{
	CustomClient c;
	c.Connect("community.onelonecoder.com", 60000);
	c.CastSpell(2.0f, 5.0f);

	return 0;
}