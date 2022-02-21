/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Serialization
 * \File:    le_serialization.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_SERIALIZATION_H
#define LE_SERIALIZATION_H

// LE Includes
#include "../Base/le_global.h"

// Boost Includes
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/array.hpp>

#define LE_DECLARE_SERIALIZABLE \
	friend class boost::serialization::access;

#endif // !LE_SERIALIZATION_H