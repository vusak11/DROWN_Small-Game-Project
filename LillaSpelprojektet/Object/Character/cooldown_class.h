#ifndef COOLDOWN_CLASS_H
#define COOLDOWN_CLASS_H

class CooldownClass {
private:
	float total_cooldown_;
	float remaining_cooldown_;
public:
	CooldownClass(float in_total_cooldown);
	~CooldownClass();

	void BeginCooldown();
	void ResetCooldown();
	bool UpdateCooldown(const float& in_deltatime);	//Returns true if the cooldown reaches 0
	bool IsOffCooldown();
};

#endif // !COOLDOWN_CLASS_H
