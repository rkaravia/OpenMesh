/*===========================================================================*\
 *                                                                           *
 *                               OpenMesh                                    *
 *      Copyright (C) 2001-2014 by Computer Graphics Group, RWTH Aachen      *
 *                           www.openmesh.org                                *
 *                                                                           *
 *---------------------------------------------------------------------------* 
 *  This file is part of OpenMesh.                                           *
 *                                                                           *
 *  OpenMesh is free software: you can redistribute it and/or modify         * 
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  OpenMesh is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with OpenMesh.  If not,                                    *
 *  see <http://www.gnu.org/licenses/>.                                      *
 *                                                                           *
\*===========================================================================*/ 

/*===========================================================================*\
 *                                                                           *             
 *   $Revision: 990 $                                                         *
 *   $Date: 2014-02-05 10:01:07 +0100 (Mi, 05 Feb 2014) $                   *
 *                                                                           *
\*===========================================================================*/


//=== INCLUDES ================================================================

#include <OpenMesh/Core/System/config.h>
#include <OpenMesh/Core/IO/reader/BaseReader.hh>
#include <algorithm>
#include <string>
#include <iterator>

#ifdef _WIN32
  #include <codecvt>
  #include <locale>
#endif

#if defined(OM_CC_MIPS)
#  include <ctype.h>
#else
#  include <cctype>
#endif


//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace IO {


//=== IMPLEMENTATION ==========================================================


static inline char tolower(char c) 
{
  using namespace std;
  return ::tolower(c); 
}


//-----------------------------------------------------------------------------


bool 
BaseReader::
can_u_read(const std::string& _filename) const 
{
  // get file extension
  std::string extension;
  std::string::size_type pos(_filename.rfind("."));

  if (pos != std::string::npos)
  { 
    extension = _filename.substr(pos+1, _filename.length()-pos-1);

    std::transform( extension.begin(), extension.end(), 
		    extension.begin(), tolower );
  }

  // locate extension in extension string
  return (get_extensions().find(extension) != std::string::npos);
}


//-----------------------------------------------------------------------------


bool 
BaseReader::
check_extension(const std::string& _fname, const std::string& _ext) const
{
  std::string cmpExt(_ext);

  std::transform( _ext.begin(), _ext.end(),  cmpExt.begin(), tolower );

  std::string::size_type pos(_fname.rfind("."));

  if (pos != std::string::npos && !_ext.empty() )
  { 
    std::string ext;

    // extension without dot!
    ext = _fname.substr(pos+1, _fname.length()-pos-1);

    std::transform( ext.begin(), ext.end(), ext.begin(), tolower );
    
    return ext == cmpExt;
  }
  return false;  
}


//-----------------------------------------------------------------------------


bool
BaseReader::
openRead(const std::string& _filename, Options _opt, std::ifstream& _is) const
{
  std::ios_base::openmode mode = (_opt.is_binary()
                                  ? std::ios_base::in | std::ios_base::binary
                                  : std::ios_base::in);
#ifdef _WIN32
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  // convert UTF-8 to UTF-16
  const std::wstring path_str = converter.from_bytes(_filename);
#else
  const std::string& path_str = _filename;
#endif
  _is.open(path_str.c_str(), mode);
  return _is.is_open();
}



//=============================================================================
} // namespace IO
} // namespace OpenMesh
//=============================================================================
