#pragma once

#include <string>
#include <vector>
#include <limits>

/*
	for (size_t i = 0; i < 5; i++) {
		cubebp.lodManager.addLod(&cubebp, (i + 1) * 100);
	}
	INFO("Current LOD : " << (int)cubebp.lodManager.getCurrentLod() << "\n");
	INFO("Current LOD : " << (int)cubebp.lodManager.setCurrentLod(2) << "\n");
	INFO("Current LOD : " << (int)cubebp.lodManager.setCurrentLod(35) << "\n");
	INFO(cubebp.lodManager.toString());
	std::vector<Lod>	removedLod;
	removedLod = cubebp.lodManager.removeLod(2);
	for (auto lod : removedLod) { INFO("removed lod : " << lod.toString() << "\n"); }
	INFO(cubebp.lodManager.toString());
	removedLod = cubebp.lodManager.removeLod(55);
	for (auto lod : removedLod) { INFO("removed lod : " << lod.toString() << "\n"); }
	INFO(cubebp.lodManager.toString());
	std::exit(0);
*/

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

class LodManager
{
public:
	LodManager();
	LodManager(Blueprint* bp);
	LodManager(const LodManager& src);
	LodManager& operator=(const LodManager& rhs);
	~LodManager();

	uint8_t				setCurrentLod(uint8_t lod);
	uint8_t				updateCurrentLod(float distance);
	bool				addLod(Blueprint* bp, float minDistance);
	std::vector<Lod>	removeLod(size_t n);
	void				changeLodBlueprintAtDistance(float distance, Blueprint* bp);
	void				changeLodBlueprint(uint8_t lod, Blueprint* bp);

	Lod					getLodDataAtDistance(float distance) const;
	Lod					getLodData(uint8_t lod) const;
	Blueprint*			getCurrentLodBlueprint() const;
	uint8_t				getCurrentLod() const;
	uint8_t				getLodCount() const;
	std::string			toString() const;

private:
	uint8_t				_getLodIndex(float distance) const;

	uint8_t				_currentLod = 0;
	std::vector<Lod>	_lods;
};
