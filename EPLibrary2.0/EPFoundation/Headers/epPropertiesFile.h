/*! 
@file epPropertiesFile.h
@author Woong Gyu La a.k.a Chris. <juhgiyo@gmail.com>
@date October 01, 2011
@brief Properties File Interface
@version 2.0

@section LICENSE

Copyright (C) 2012  Woong Gyu La <juhgiyo@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

@section DESCRIPTION

An Interface for the Properties File Class.

*/
#ifndef __EP_PROPERTIES_FILE_H__
#define __EP_PROPERTIES_FILE_H__
#include "epLib.h"
#include "epSystem.h"
#include "epMemory.h"
#include "epBaseFile.h"
#include <list>

using namespace std;

#ifndef TEST_NEW
#define TEST_NEW 1
#endif //TEST_NEW

namespace epl{

#if TEST_NEW
	/*!
	@class PropertiesFile epPropertiesFile.h
	@brief A class for Peroperties File.
	*/
	class EP_FOUNDATION PropertiesFile:public BaseFile{
	public:

		/*!
		Default Constructor

		Initializes the Properties File 
		@param[in] encodingType the encoding type for this file
		*/
		PropertiesFile(FileEncodingType encodingType=FILE_ENCODING_TYPE_UTF16);

		/*!
		Default Destructor

		Destroy the Properties File 
		*/
		virtual ~PropertiesFile();

		/*!
		Set the property with the given key with the value given
		@param[in] key the key of the property to change the value
		@param[in] val the value to change the property
		@return true if changed, otherwise false
		*/
		bool SetProperty(CString key, CString val);

		/*!
		Get the value of the property with the given key
		@param[in] key the key of the property to get the value
		@param[in] retVal the value of the property of given key
		@return true if found, otherwise false
		*/
		bool GetProperty(CString key,CString &retVal);

		/*!
		Add new property with the given key and value
		@param[in] key the key of the property to add
		@param[in] val the value of the new property
		@return true if successfully added, otherwise false
		*/
		bool AddProperty(CString key, CString val);

		/*!
		Remove the property with the given key
		@param[in] key the key of the property to remove
		@return true if successfully removed, otherwise false
		*/
		bool RemoveProperty(CString key);

		/*!
		Clear the list of the properties
		*/
		void Clear();

		/*!
		Save the list of the properties from the given file
		@param[in] fileName the name of the file to save the list of properties
		@return true if successfully saved, otherwise false
		*/
		bool SaveToFile(CString filename);
		
		/*!
		Load the list of the properties from the given file
		@param[in] fileName the name of the file to load the list of properties
		@return true if successfully loaded, otherwise false
		*/
		bool LoadFromFile(CString filename);

	private:

		/*!
		Loop Function that writes to the file
		** Sub classes should implement this function
		*/
		virtual void writeLoop();

		/*!
		Actual Load Function that loads values from the file
		** Sub classes should implement this function
		*/
		virtual void loadFromFile(CString line);

		/*!
		Parse the key and value from the line buffer
		@param[in] buf the buffer that holds a line
		@param[out] retKey the key part of the given line
		@param[out] retVal the value part of the given line
		@return true if successfully parsed the key and value, otherwise false
		*/
		bool getValueKeyFromLine(CString buf, CString &retKey, CString &retVal);

		/// The list of the properties
		list<pair<CString,CString> > m_propertyList;
	};
#else
	/*!
	@class PropertiesFile epPropertiesFile.h
	@brief A class for Peroperties File.
	*/
	class EP_FOUNDATION PropertiesFile{
	public:

		/*!
		Default Constructor

		Initializes the Properties File 
		@param[in] encodingType the encoding type for this file
		*/
		PropertiesFile(FileEncodingType encodingType=FILE_ENCODING_TYPE_UTF16);

		/*!
		Default Destructor

		Destroy the Properties File 
		*/
		virtual ~PropertiesFile();

		/*!
		Set the property with the given key with the value given
		@param[in] key the key of the property to change the value
		@param[in] val the value to change the property
		@return true if changed, otherwise false
		*/
		bool SetProperty(CString key, CString val);

		/*!
		Get the value of the property with the given key
		@param[in] key the key of the property to get the value
		@param[in] retVal the value of the property of given key
		@return true if found, otherwise false
		*/
		bool GetProperty(CString key,CString &retVal);

		/*!
		Add new property with the given key and value
		@param[in] key the key of the property to add
		@param[in] val the value of the new property
		@return true if successfully added, otherwise false
		*/
		bool AddProperty(CString key, CString val);

		/*!
		Remove the property with the given key
		@param[in] key the key of the property to remove
		@return true if successfully removed, otherwise false
		*/
		bool RemoveProperty(CString key);

		/*!
		Clear the list of the properties
		*/
		void Clear();

		/*!
		Save the list of the properties from the given file
		@param[in] filename the name of the file to save the list of properties
		@return true if successfully saved, otherwise false
		*/
		bool SaveToFile(CString filename);
		
		/*!
		Load the list of the properties from the given file
		@param[in] filename the name of the file to load the list of properties
		@return true if successfully loaded, otherwise false
		*/
		bool LoadFromFile(CString filename);

	private:
		/*!
		Get a single line from the given buffer
		@param[in] buf the buffer that holds all lines
		@param[out] retLine the first line that found in the given buffer
		@param[out] retRest the rest of the buffer without the found line
		@return true if successfully parsed the line, otherwise false
		*/
		bool getLine(CString buf, CString &retLine, CString &retRest);

		/*!
		Parse the key and value from the line buffer
		@param[in] buf the buffer that holds a line
		@param[out] retKey the key part of the given line
		@param[out] retVal the value part of the given line
		@return true if successfully parsed the key and value, otherwise false
		*/
		bool getValueKeyFromLine(CString buf, CString &retKey, CString &retVal);

		/// The list of the properties
		list<pair<CString,CString> > m_propertyList;

		/// Encoding type of the file
		FileEncodingType m_encodingType;
	};
#endif
}



#endif //__EP_PROPERTIES_FILE_H__