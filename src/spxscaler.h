/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                  This file is part of the class library                   */
/*       SoPlex --- the Sequential object-oriented simPlex.                  */
/*                                                                           */
/*    Copyright (C) 1996-2016 Konrad-Zuse-Zentrum                            */
/*                            fuer Informationstechnik Berlin                */
/*                                                                           */
/*  SoPlex is distributed under the terms of the ZIB Academic Licence.       */
/*                                                                           */
/*  You should have received a copy of the ZIB Academic License              */
/*  along with SoPlex; see the file COPYING. If not email to soplex@zib.de.  */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**@file  spxscaler.h
 * @brief LP scaling base class.
 */
#ifndef _SPXSCALER_H_
#define _SPXSCALER_H_

#include <assert.h>

#include "spxdefines.h"
#include "dataarray.h"
#include "vector.h"
#include "svector.h"
#include "svset.h"
#include "dsvector.h"
#include "dvector.h"

namespace soplex
{

template < class R >
class SPxLPBase;
/**@brief   LP scaler abstract base class.
   @ingroup Algo

   Instances of classes derived from SPxScaler may be loaded to SoPlex in
   order to scale LPs before solving them. SoPlex will load() itself to
   the SPxScaler and then call #scale(). Generally any SPxLP can be
   loaded to a SPxScaler for #scale()%ing it. The scaling can
   be undone by calling unscale().
*/

class SPxScaler
{
protected:

   //-------------------------------------
   /**@name Data */
   //@{
   const char*        m_name;      ///< Name of the scaler
#if 0
   DataArray < Real > m_colscale;  ///< column scaling factors
   DataArray < Real > m_rowscale;  ///< row scaling factors
#endif
   DataArray < int >  m_colscaleExp;  ///< inner (post-presolving) column scaling factors
   DataArray < int >  m_rowscaleExp;  ///< inner (post-presolving) row scaling factors
   DataArray < int >  m_colscaleExpPersistent;  ///< persistent (or outer) column scaling factors
   DataArray < int >  m_rowscaleExpPersistent;  ///< persistent (or outer) row scaling factors
   DataArray < int >* m_activeColscaleExp; ///< pointer to currently active column scaling factors
   DataArray < int >* m_activeRowscaleExp; ///< pointer to currently active row scaling factors
   bool               m_colFirst;  ///< do column scaling first 
   bool               m_doBoth;    ///< do columns and rows
   bool               m_usingPersistentFactors; ///<  are persistent scaling factors being used?
   SPxOut*            spxout;      ///< message handler
   //@}

   //-------------------------------------
   /**@name Protected helpers */
   //@{

   /// set pointer to currently active scaling factors (persistent/outer or inner)
   void setActiveScalingExp(bool persistent);

   /// setup scale array for the LP.
   virtual void setup(SPxLPBase<Real>& lp);
#if 0
   /// computes scaling value for a minimum and maximum pair.
   virtual Real computeScale(Real mini, Real maxi) const;

   /// iterates through vecset and calls computeScale() for each vector.
   /**@return maximum ratio between absolute biggest and smallest element for any vector.
    */
   virtual Real computeScalingVecs( const SVSet* vecset, 
                                    const DataArray<int>& coScaleExp,
                                    DataArray<int>& scaleExp );
#endif
   //@}

public:

   /// are persistent scaling factors being used?
   bool usingPersistentFactors();

   /// applies m_colscale and m_rowscale to the \p lp.
   virtual void applyScaling(SPxLPBase<Real>& lp);

   friend std::ostream& operator<<(std::ostream& s, const SPxScaler& sc);

   //-------------------------------------
   /**@name Construction / destruction */
   //@{
   /// constructor
   explicit SPxScaler(const char* name, bool colFirst = false, bool doBoth = true, SPxOut* spxout = NULL);
   /// copy constructor
   SPxScaler(const SPxScaler& );
   /// assignment operator
   SPxScaler& operator=(const SPxScaler& );
   /// destructor.
   virtual ~SPxScaler();
   /// clone function for polymorphism
   virtual SPxScaler* clone() const = 0;
   //@}

   //-------------------------------------
   /**@name Access / modification */
   //@{
   /// get name of scaler
   virtual const char* getName() const;
   /// set scaling order
   virtual void setOrder(bool colFirst); 
   /// set wether column and row scaling should be performed
   virtual void setBoth(bool both); 
   /// set message handler
   virtual void setOutstream(SPxOut& newOutstream)
   {
      spxout = &newOutstream;
   }
   /// set real parameter
   virtual void setRealParam(Real param, const char* name = "realparam");
   /// set int parameter
   virtual void setIntParam(int param, const char* name = "intparam");
   //@}

   //-------------------------------------
   /**@name Scaling */
   //@{
   /// scale SPxLP.
   virtual void scale(SPxLPBase<Real>& lp, bool persistent = true) = 0;
   /// unscale SPxLP
   virtual void unscale(SPxLPBase<Real>& lp, bool persistent = true);
   /// returns scaling factor for column \p i
   virtual int getColScaleExp(int i);
   /// returns scaling factor for row \p i
   virtual int getRowScaleExp(int i);
   /// gets unscaled column \p i
   virtual void getColUnscaled(const SPxLPBase<Real>& lp, int i, SVector& vec) const;
   /// returns unscaled upper bound \p i
   virtual Real upperUnscaled(const SPxLPBase<Real>& lp, int i) const;
   /// returns unscaled upper bound vector of LP \lp
   virtual void getUpperUnscaled(const SPxLPBase<Real>& lp, DVector& vec) const;
   /// returns unscaled lower bound \p i
   virtual Real lowerUnscaled(const SPxLPBase<Real>& lp, int i) const;
   /// gets unscaled lower bound vector
   virtual void getLowerUnscaled(const SPxLPBase<Real>& lp, Vector& vec) const;
   /// returns unscaled objective function coefficient of \p i
   virtual Real maxObjUnscaled(const SPxLPBase<Real>& lp, int i) const;
   /// gets unscaled objective function
   virtual void getMaxObjUnscaled(const SPxLPBase<Real>& lp, Vector& vec) const;
   /// returns unscaled row \p i
   virtual void getRowUnscaled(const SPxLPBase<Real>& lp, int i, SVector& vec) const;
   /// returns unscaled right hand side \p i
   virtual Real rhsUnscaled(const SPxLPBase<Real>& lp, int i) const;
   /// gets unscaled right hand side vector
   virtual void getRhsUnscaled(const SPxLPBase<Real>& lp, Vector& vec) const;
   /// returns unscaled left hand side \p i of LP \lp
   virtual Real lhsUnscaled(const SPxLPBase<Real>& lp, int i) const;
   /// returns unscaled left hand side vector of LP \lp
   virtual void getLhsUnscaled(const SPxLPBase<Real>& lp, Vector& vec) const;
   /// unscale dense primal solution vector given in \p x. 
   virtual void unscalePrimal(Vector& x) const;
   /// unscale dense slack vector given in \p s.
   virtual void unscaleSlacks(Vector& s) const;
   /// unscale dense dual solution vector given in \p pi. 
   virtual void unscaleDual(Vector& pi) const;
   /// unscale dense reduced cost vector given in \p r.
   virtual void unscaleRedCost(Vector& r) const;
   /// absolute smallest column scaling factor
   virtual Real minAbsColscale() const;
   /// absolute biggest column scaling factor
   virtual Real maxAbsColscale() const;
   /// absolute smallest row scaling factor
   virtual Real minAbsRowscale() const;
   /// absolute biggest row scaling factor
   virtual Real maxAbsRowscale() const;
   /// maximum ratio between absolute biggest and smallest element in any column.
   virtual Real maxColRatio(const SPxLPBase<Real>& lp) const;
   /// maximum ratio between absolute biggest and smallest element in any row.
   virtual Real maxRowRatio(const SPxLPBase<Real>& lp) const;
   //@}

   //-------------------------------------
   /**@name Debugging */
   //@{
   /// consistency check
   virtual bool isConsistent() const;
   //@}
};
} // namespace soplex
#endif // _SPXSCALER_H_
