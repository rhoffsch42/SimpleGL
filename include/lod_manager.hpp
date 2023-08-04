#pragma once

#include <string>
#include <vector>
#include <limits>

class Blueprint;//do not include blueprint.hpp

class Lod
{
public:
	Lod() = delete;
	Lod(Blueprint* bp, float min = -1.0f, float max = std::numeric_limits<float>::infinity());
	Lod(const Lod& bp);
	Lod& operator=(const Lod& rhs);
	~Lod();

	std::string	toString() const;

	Blueprint*	blueprint;
	float		minDistance = -1.0f;
	float		maxDistance = std::numeric_limits<float>::infinity();
private:
};

#define	LOD_MANAGER_DEEP_DTOR_ON	true
#define	LOD_MANAGER_DEEP_DTOR_OFF	false

class LodManager
{
public:
	LodManager() = delete;
	LodManager(Blueprint* bp, bool deep_destruction = false);
	LodManager(const LodManager& src);
	LodManager& operator=(const LodManager& rhs);
	~LodManager();

	uint8_t				setCurrentLod(uint8_t lod);
	uint8_t				updateCurrentLod(float distance);
	bool				addLod(Blueprint* bp, float minDistance);
	std::vector<Lod>	removeLod(size_t amount);
	void				changeLodBlueprintAtDistance(float distance, Blueprint* bp);
	void				changeLodBlueprint(uint8_t lod, Blueprint* bp);

	Lod					getLodDataAtDistance(float distance) const;
	Lod					getLodData(uint8_t lod) const;
	Blueprint*			getCurrentLodBlueprint() const;
	uint8_t				getCurrentLod() const;
	uint8_t				getLodCount() const;
	std::string			toString() const;

	bool				deepDestruction = false;
private:
	uint8_t				_getLodIndex(float distance) const;

	uint8_t				_currentLod = 0;
	std::vector<Lod>	_lods;
};
