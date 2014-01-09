/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                  This file is part of the class library                   */
/*       SoPlex --- the Sequential object-oriented simPlex.                  */
/*                                                                           */
/*    Copyright (C) 1996-2014 Konrad-Zuse-Zentrum                            */
/*                            fuer Informationstechnik Berlin                */
/*                                                                           */
/*  SoPlex is distributed under the terms of the ZIB Academic Licence.       */
/*                                                                           */
/*  You should have received a copy of the ZIB Academic License              */
/*  along with SoPlex; see the file COPYING. If not email to soplex@zib.de.  */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <iostream>
#include <assert.h>

#include "statistics.h"

namespace soplex
{
   /// default constructor
   SoPlex::Statistics::Statistics()
   {
      clearAllData();
   }

   /// clears all statistics
   void SoPlex::Statistics::clearAllData()
   {
      readingTime.reset();
      clearSolvingData();
   }

   /// clears statistics on solving process
   void SoPlex::Statistics::clearSolvingData()
   {
      solvingTime.reset();
      preprocessingTime.reset();
      simplexTime.reset();
      syncTime.reset();
      transformTime.reset();
      luFactorizationTime = 0.0;
      luSolveTime = 0.0;
      iterations = 0;
      luFactorizations = 0;
      luSolves = 0;
      refinements = 0;
      stallRefinements = 0;
   }

   /// prints statistics
   void SoPlex::Statistics::print(std::ostream& os)
   {
      Real solTime = solvingTime.userTime();
      Real totTime = readingTime.userTime() + solTime;
      Real otherTime = solTime - syncTime.userTime() - transformTime.userTime() - preprocessingTime.userTime() - simplexTime.userTime();

      os << std::fixed << std::setprecision(2);

      os << "Total time         : " << totTime << "\n"
         << "  Reading          : " << readingTime.userTime() << "\n"
         << "  Solving          : " << solTime << "\n"
         << "  Preprocessing    : " << preprocessingTime.userTime();
      if( solTime > 0 )
         os << " (" << 100 * (preprocessingTime.userTime() / solTime) << "% of solving time)";
      os << "\n  Simplex          : " << simplexTime.userTime();
      if( solTime > 0 )
         os << " (" << 100 * (simplexTime.userTime() / solTime) << "% of solving time)";
      os << "\n  Synchronization  : " << syncTime.userTime();
      if( solTime > 0 )
         os << " (" << 100 * (syncTime.userTime() / solTime) << "% of solving time)";
      os << "\n  Transformation   : " << transformTime.userTime();
      if( solTime > 0 )
         os << " (" << 100*transformTime.userTime() / solTime << "% of solving time)";
      os << "\n  Other            : " << otherTime;
      if( solTime > 0  )
         os << " (" << 100*otherTime / solTime << "% of solving time)";

      os << "\nRefinements        : " << refinements << "\n"
         << "  Stalling         : " << stallRefinements << "\n";

      os << "Iterations         : " << iterations << "\n"
         << "  From scratch     : " << iterations - iterationsFromBasis;
      if( iterations > 0 )
         os << " (" << 100*double((iterations - iterationsFromBasis))/double(iterations) << "%)";
      os << "\n  From basis       : " << iterationsFromBasis;
      if( iterations > 0 )
         os << " (" << 100*double(iterationsFromBasis)/double(iterations) << "%)";
      os << "\n  Primal           : " << iterationsPrimal;
      if( iterations > 0 )
         os << " (" << 100*double(iterationsPrimal)/double(iterations) << "%)";
      os << "\n  Dual             : " << iterations - iterationsPrimal;
      if( iterations > 0 )
         os << " (" << 100*double((iterations - iterationsPrimal))/double(iterations) << "%)";

      os << "\nLU factorizations  : " << luFactorizations << "\n"
         << "  Factor. frequency: ";
      if( luFactorizations > 0 )
         os << double(iterations) / double(luFactorizations) << " iterations per factorization\n";
      else
         os << "-\n";
      os << "  Factor. time     : " << luFactorizationTime << "\n";

      os << "LU solves          : " << luSolves << "\n"
         << "  Solve frequency  : ";
      if( luSolves > 0 )
         os << double(luSolves) / double(iterations) << " solves per iteration\n";
      else
         os << "-\n";
      os << "  Solve time       : " << luSolveTime << "\n";
   }
} // namespace soplex
