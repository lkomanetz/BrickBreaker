#include "GameObject.h"

GameObject::GameObject() :
	_health(0) {
}

GameObject::GameObject(const GameObject& obj) :
	Image(obj) {
	
	_health = obj._health;
}

GameObject& GameObject::operator=(const GameObject& obj) {
	Image::operator=(obj);
	_health = obj._health;

	return *this;
}
