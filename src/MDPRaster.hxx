
#ifndef __MODELX_MDPRASTER_HXX__
#define __MODELX_MDPRASTER_HXX__

#include <map>
#include <memory>
#include <HashTable.hxx>
#include <DynamicRaster.hxx>
#include <Size.hxx>
#include <Environment.hxx>
#include <Exception.hxx>

namespace Model
{

/**
 * 
 * 
 * Note that an MDPRaster DOES NOT subclass any of the raster classes, for performance reasons.
 */
class MDPRaster
{
protected:
	//! A constant raster that we take as the base upon which individual cell modifications
	//! are stored separately for performance reasons
	std::shared_ptr<const Engine::DynamicRaster> baseRaster;
	
	//! A map with all the specific modifications made to the base raster.
	//! The container must be an ordered map for the hash function to work as expected.
	std::map<Engine::Point2D<int>, int> modified;
	
	//! 
	int delta;

public:
	
	//! MDP Rasters can only be constructed from a dynamic raster
	MDPRaster(const Engine::DynamicRaster& baseRaster) :
		// We make a full copy of the dynamic raster (just in case it gets erased or modified) and then get a smart pointer of it.
		baseRaster( std::make_shared<const Engine::DynamicRaster>(baseRaster) ),
		modified(),
		delta(0)
	{}
	
	//! Copy constructor
	MDPRaster(const MDPRaster& other) :
		baseRaster( other.baseRaster ),
		modified( other.modified), // Copy the whole list of modifications
		delta(0)
	{}

	virtual ~MDPRaster() {}
	
	//! Consume a given value to the given position.
	void add(const Engine::Point2D<int> & pos, int value ) {
		int max = getMax(pos);
		auto it = modified.find(pos);
		
		if (it == modified.end()) { // The value was yet unaffected
			int base = std::min(max, baseRaster->getValue(pos) + delta); // This is computeUnaffectedCellValue(pos), but keeping the max for greater performance
			int& current = modified.insert(std::make_pair(pos, base)).first->second; // Insert the element and get a reference to the int value
			current = computeValue(current, value, max);
		
		} else { // The value was already affected
			it->second = computeValue(it->second, value, max);
		}
	}
	
	
	//! Adds a value to an already modified element.
	int computeValue(int current, int value, int max) {
		int future = current + value;
		// Keep the invariant: current needs to hold the delta between the base value and the values accumulated
		// with time, but always up to a given maximum
		if (future < 0) throw Engine::Exception("Cannot add " + std::to_string(value) + " to an MDPRaster cell containing only " + std::to_string(current) + " units.");
		else if (future <= max) return future;
		else /* future > max */ return  max;
	}
	
	//!
	void addAll(int value) {
		delta += value;
		for (auto& change:modified) {
			change.second = computeValue(change.second, value, getMax(change.first));
		}
	}
	
	//! 
	int computeUnaffectedCellValue(const Engine::Point2D<int>& pos) const {
		return std::min(getMax(pos), baseRaster->getValue(pos) + delta);
	}
	
	
	//! 
	int at(const Engine::Point2D<int> & pos) const {
		const auto it = modified.find(pos);
		
		if (it == modified.end()) { // The value is unaffected
			return computeUnaffectedCellValue(pos);
		} else { // The true value is on the map of modifications
			return it->second;
		}
	}
	
	
	//!
	int at(int x, int y) const { return at(Engine::Point2D<int>(x,y)); }
	
	inline int getMax(const Engine::Point2D<int>& pos) const { return baseRaster->getMaxValue(pos);}


	//! Comparison operator.
	bool operator==(const MDPRaster& other) const {
		if (baseRaster != other.baseRaster) return false;
		if (delta != other.delta) return false;  // This is overly restrictive, but we sacrifice completeness this for performance.
		if (modified != other.modified) return false;
		return true;
	}
	
	bool operator!=( const MDPRaster& other ) const { return !(this->operator==( other)); }
	
	//! Hash the resource raster - 
	unsigned hash() const {
		Engine::HashKey hashKey;
		
		// Hash the pointer value. We consider MDPRasters deriving from different base rasters to be different, even 
		// if their contents are the same
		hashKey.add((long) baseRaster.get()); // This is a bit unorthodox, but we are content with it.
		hashKey.add(delta);

		// Hash all the modifications. Note that this works because the container is an ordered map.
		for (auto change:modified) {
			hashKey.add(change.first._x);
			hashKey.add(change.first._y);
			hashKey.add(change.second);
		}
		
		return hashKey.code();
	}
	
	//! Assignment operator
	MDPRaster& operator=(const MDPRaster& other) {
		if (this != &other) {
			baseRaster = other.baseRaster;
			modified = other.modified;
			delta = other.delta;
        }
        return *this;
	}	
};

}

#endif
