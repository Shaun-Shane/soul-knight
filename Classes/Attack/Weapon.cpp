#include "Weapon.h"

Weapon::Weapon() {}

bool Weapon::init() { return true; }

Weapon::~Weapon() {}

void Weapon::setFireSpeed(float fireSpeed) { this->fireSpeed = fireSpeed; }

float Weapon::getFireSpeed() { return this->fireSpeed; }

void Weapon::setAttack(INT32 attack) { this->attack = attack; }

INT32  Weapon::getAttack() { return this->attack; }

INT32 Weapon::getMPConsumption() { return mpConsumption; }

void Weapon::setMPConsumption(INT32 deta) { this->mpConsumption = deta; }

bool Weapon::getWeaponState() { return this->weaponState; }

void Weapon::setWeaponState(bool state) { this->weaponState = state; }

void Weapon::setBulletType(INT32 type) { this->bulletType = type; }

void Weapon::knifeAttack(Knight* knight)
{
	if (knight->getAtBattleRoom() == nullptr) return;
	Vector<Enemy*>& vecEnemy = knight->getAtBattleRoom()->getVecEnemy();

	for (auto& e : vecEnemy) {
		if (e->getParent() == nullptr) continue;

		float enemyX = e->getPositionX(), enemyY = e->getPositionY();

		if (sqrt(pow(knight->getPositionX() - enemyX, 2) +
			pow(knight->getPositionY() - enemyY, 2)) <= 80.0f) {
			e->deductHP(this->attack * knight->getDamageBuff());
		}
	}
}

Bullet* Weapon::createBullet(Vec2 speed, INT32 firePower, bool isBig)
{
	Bullet* bullet = Bullet::create();
	bullet->setBulletSpeed(speed);

	char fileName[30];
	if (isBig == false) {
		bullet->setAttack(firePower);
		sprintf(fileName, "Bullet//bullet_%d.png", this->bulletType);
	}
	else {
		bullet->setAttack(firePower * 3);
		sprintf(fileName, "Bullet//bigBullet.png");
	}
	bullet->bindSprite(Sprite::create(fileName), 12);

	if (speed.x == 0 && speed.y > 0) bullet->getSprite()->setRotation(-90.0f);
	else if (speed.x < 0)    bullet->getSprite()->setRotation(-(180.0f + 180.0f * atan(speed.y / speed.x) / PI));
	else if (speed.x == 0 && speed.y < 0)       bullet->getSprite()->setRotation(-(180.0f + 180.0f * atan(speed.y / speed.x) / PI));
	else    bullet->getSprite()->setRotation(-(180.0f + 180.0f * atan(speed.y / speed.x) / PI));
	return bullet;
}

void Weapon::weaponInit(float speed, INT32 weaponAttack, INT32 decMP, int weaponType, bool state, int bulletType) {
	setFireSpeed(speed);
	setAttack(weaponAttack);
	setMPConsumption(decMP);
	char fileName[30]; sprintf(fileName, "Weapon//weapon%d.png", weaponType);
	bindSprite(Sprite::create(fileName), LayerPlayer);
	setWeaponState(state);
	setBulletType(bulletType);
}
