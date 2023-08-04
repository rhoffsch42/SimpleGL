#include "lod_manager.hpp"
#include "blueprint.hpp"

#include <sstream>
#include <iostream>

//#define SGL_DEBUG
#ifdef SGL_DEBUG
#define SGL_LOD_MANAGER_DEBUG
#endif
#ifdef SGL_LOD_MANAGER_DEBUG 
#define D(x) std::cout << "[LodManager] " << x
#define D_(x) x
#define D_SPACER "-- lod_manager.cpp -------------------------------------------------\n"
#define D_SPACER_END "----------------------------------------------------------------\n"
#else 
#define D(x)
#define D_(x)
#define D_SPACER ""
#define D_SPACER_END ""
#endif

Lod::Lod(Blueprint* bp, float min, float max) : blueprint(bp), minDistance(min), maxDistance(max) {}
Lod::Lod(const Lod& blueprint) { *this = blueprint; }
Lod::~Lod() {}
Lod& Lod::operator=(const Lod& rhs) {
	this->blueprint = rhs.blueprint;
	this->minDistance = rhs.minDistance;
	this->maxDistance = rhs.maxDistance;
	return *this;
}
std::string	Lod::toString() const {
	std::stringstream ss;
	ss << this->minDistance << "\t->\t" << this->maxDistance << "\t" << this->blueprint << "\t" << this->blueprint->getName();
	return ss.str();
}

//LodManager::LodManager() {}//deleted
LodManager::LodManager(Blueprint* obj, bool deep_destruction) : deepDestruction(deep_destruction) {
	this->_lods.push_back(Lod(obj));
}
LodManager::LodManager(const LodManager& src) {
	*this = src;
}

// [Checklist] Make sure that if you copy a LodManager with deepDestruction to true, to not let them delete the same BPs at their death.
LodManager&			LodManager::operator=(const LodManager& rhs) {
	this->_currentLod = rhs._currentLod;
	this->_lods = rhs._lods;
	this->deepDestruction = rhs.deepDestruction;
	return *this;
}
LodManager::~LodManager() {
	//D("~LodManager() " << this << "\n");
	if (this->deepDestruction) {
		// the first one is not deleted (it is the owning BP)
		for (size_t i = 1; i < this->_lods.size(); i++) {
			//D("deleting LOD_" << i << "\t" << this->_lods[i].blueprint << "\n");
			delete this->_lods[i].blueprint;
		}
	}
}

uint8_t				LodManager::setCurrentLod(uint8_t lod) {
	this->_currentLod = std::min(lod, (uint8_t)(this->_lods.size() - 1));
	return this->_currentLod;
}

uint8_t				LodManager::updateCurrentLod(float distance) {
	return this->setCurrentLod(this->_getLodIndex(distance));
}

// Arbitrary max size of 255
bool				LodManager::addLod(Blueprint* obj, float minDistance) {
	if (this->_lods.size() == 255) {
		std::cout << "[LodManager] Warning: Blueprint " << this << " failed to add new LOD, array is full (255).\n";
		return false;
	}
	float min = this->_lods.back().minDistance;
	if (min > minDistance) {
		std::cout << "[LodManager] Warning: Blueprint " << this << " created discontinuity with the last and new lod: " << min << " > " << minDistance << "\n";
	}
	this->_lods.back().maxDistance = minDistance;
	this->_lods.push_back(Lod(obj, minDistance));
	return true;
}

// removed additionnal LODs and return them
std::vector<Lod>	LodManager::removeLod(size_t amount) {
	/*
		cannot do this as the default ctor is deleted:
		n = std::max((size_t)1, this->_lods.size() - n); // keep LOD 0 (full details = this)
		this->_lods.resize(n);
	*/
	std::vector<Lod>	removed;
	while (amount && this->_lods.size() > 1) {
		removed.push_back(this->_lods.back());
		this->_lods.pop_back();
		amount--;
	}
	this->_lods.back().maxDistance = std::numeric_limits<float>::infinity();
	this->setCurrentLod(this->_currentLod);// will update it correctly if currentLod is too high
	return removed;
}

void				LodManager::changeLodBlueprintAtDistance(float distance, Blueprint* bp) {
	this->_lods[this->_getLodIndex(distance)].blueprint = bp;
}
void				LodManager::changeLodBlueprint(uint8_t lod, Blueprint* bp) {
	this->_lods[lod].blueprint = bp;
}

uint8_t				LodManager::_getLodIndex(float distance) const {
	size_t l = 0;
	for (size_t i = 0; i < this->_lods.size(); i++) {
		if (this->_lods[i].minDistance <= distance)
			l = i;
		else
			break;
	}
	return l;
}

Lod					LodManager::getLodDataAtDistance(float distance) const		{ return this->_lods[this->_getLodIndex(distance)]; }
Lod					LodManager::getLodData(uint8_t lod) const					{ return this->_lods[lod]; }
Blueprint*			LodManager::getCurrentLodBlueprint() const					{ return this->_lods[this->_currentLod].blueprint; }
uint8_t				LodManager::getCurrentLod() const							{ return this->_currentLod; }
uint8_t				LodManager::getLodCount() const								{ return this->_lods.size(); }
std::string			LodManager::toString() const {
	std::stringstream ss;
	int i = 0;
	ss << "Current LOD " << (int)this->_currentLod << ", LODs models for " << this << ": \n";
	for (auto lod : this->_lods) {
		ss << "LOD_" << i << ":\t" << lod.toString() << "\n";
		i++;
	}
	ss << "deep destruction: " << (this->deepDestruction ? "true" : "false") << "\n";
	return ss.str();
}
