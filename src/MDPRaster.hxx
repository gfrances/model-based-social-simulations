
#ifndef __MODELX_MDPRASTER_HXX__
#define __MODELX_MDPRASTER_HXX__

#include <map>
#include <memory>
#include <HashTable.hxx>
#include <DynamicRaster.hxx>
#include <Size.hxx>
#include <Environment.hxx>
#include <Exception.hxx>
#include <Point2D.hxx>

namespace Model
{

/**
 * An MDPRaster is a derived type of raster that stores a (limited) number of modifications to individual
 * cells of a "base" raster. The main aim is to reduce the performance impact of the multiple copies
 * of raster objects that need to be done during the MDP process.
 * To that end, we only store a smart pointer to the original rasters, and individual modifications are stored
 * in a map mapping positions to the overall modification.
 * Global modifications can also occur, their effect being accumulated in a `delta` attribute.
 * 
 * The real value of any given cell of the raster is at any time either explicitly stored in the 
 * map of modifications, or, if the map is not defined on that cell, implicitly stored in the base
 * raster plus the global modifications, and always taking into account that the real value cannot
 * go beyond the maximum specified by the base raster. This means that when a cell is individually modified,
 * the global delta modifier does no longer apply to it.
 * 
 * Note that an MDPRaster DOES NOT subclass any of the raster classes, for performance reasons, i.e. to avoid
 * inheriting their attributes.
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
	
	//! A global modifier affecting all raster cells that have not been individually modified.
	int delta;

public:
	
	//! MDP Rasters can only be constructed from a dynamic raster
	explicit MDPRaster(const Engine::DynamicRaster& baseRaster) :
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
	
	//! Return the raster valye for the given cell
	int at(const Engine::Point2D<int> & pos) const {
		const auto it = modified.find(pos);
		
		if (it == modified.end()) { // The value is unaffected
			return computeUnaffectedCellValue(pos);
		} else { // The true value is on the map of modifications
			return it->second;
		}
	}
	
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
	
	//! Add a value to all raster cells.
	void addAll(int value) {
		delta += value;
		for (auto& change:modified) { // The individually modified cells need to be updated separately.
			change.second = computeValue(change.second, value, getMax(change.first));
		}
	}
	
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
	
	
	//! Prints a representation of the state to the given stream.
	//! Not the most efficient implementation, but works well for debugging purposes.
	friend std::ostream& operator<<( std::ostream &os, const MDPRaster& raster) { return raster.print(os); }
	virtual std::ostream& print(std::ostream& os) const {
		auto size = baseRaster->getSize();
		for(int i = 0; i < size._width; ++i) {
			for(int j = 0; j < size._height; ++j) {
				Engine::Point2D<int> idx(i,j);
				os << "(" << i << "," << j << "): " << at(idx) << "/" << getMax(idx) << "  ";
			}
			os << std::endl;
		}
		return os;
	}
	
	
protected:
	
	//! This method encodes the logic of the modification of alread-ymodified cells.
	int computeValue(int current, int value, int max) {
		int future = current + value;
		if (future < 0) throw Engine::Exception("Cannot add " + std::to_string(value) + " to an MDPRaster cell containing only " + std::to_string(current) + " units.");
		return std::min(future, max);
	}
	
	//! Returns the real value of any unaffected cell, modified by the delta attribute
	//! and capped not to surpass the maximum.
	int computeUnaffectedCellValue(const Engine::Point2D<int>& pos) const {
		return std::min(getMax(pos), baseRaster->getValue(pos) + delta);
	}
	
	//! A simple helper
	inline int getMax(const Engine::Point2D<int>& pos) const { return baseRaster->getMaxValue(pos);}	
};

}

#endif
