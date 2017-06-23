/**
* @version		GrPPI v0.2
* @copyright		Copyright (C) 2017 Universidad Carlos III de Madrid. All rights reserved.
* @license		GNU/GPL, see LICENSE.txt
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You have received a copy of the GNU General Public License in LICENSE.txt
* also available in <http://www.gnu.org/licenses/gpl.html>.
*
* See COPYRIGHT.txt for copyright notices and details.
*/

#ifndef GRPPI_MAP_SEQ_H
#define GRPPI_MAP_SEQ_H

namespace grppi{

/**
\addtogroup map_pattern
@{
*/

/**
\brief Invoke [map pattern](@ref map-pattern) on a data sequence.
\tparam InputIt Iterator type used for input sequence.
\tparam OtuputIt Iterator type used for the output sequence.
\tparam Operation Callable type for the transformation operation.
\param ex Sequential execution policy object
\param first Iterator to the first element in the input sequence.
\param last Iterator to one past the end of the input sequence.
\param first_out Iterator to first elemento of the output sequence.
\param op Transformation operation.
*/
template <typename InputIt, typename OutputIt, typename Operation>
void map(sequential_execution & ex, 
         InputIt first, InputIt last, OutputIt first_out, 
         Operation && op ) 
{
  while(first != last) {
    *first_out = op(*first);
    first++;
    first_out++;
  }
}

/**
\brief Invoke [map pattern](@ref map-pattern) on a data sequence.
\tparam InputIt Iterator type used for input sequence.
\tparam OtuputIt Iterator type used for the output sequence.
\tparam Operation Callable type for the transformation operation.
\param ex Sequential execution policy object
\param first Iterator to the first element in the input sequence.
\param last Iterator to one past the end of the input sequence.
\param first_out Iterator to first elemento of the output sequence.
\param op Transformation operation.
\param inputs Additional iterators with first elements of additional sequences.
*/
template <typename InputIt, typename OutputIt, typename ... MoreIn, typename Operation>
 void map(sequential_execution &s, InputIt first, InputIt last, OutputIt firstOut, Operation && op, MoreIn ... inputs ) {

    while( first != last ) {
        *firstOut = op( *first, *inputs ... );
        advance_iterators( inputs... );
        first++;
        firstOut++;
    }
}

/**
@}
*/
}
#endif
