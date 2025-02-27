//---------------------------------------------------------------------------
//	Greenplum Database
//	Copyright (C) 2016 VMware, Inc. or its affiliates.
//	Copyright (C) 2021, Alibaba Group Holding Limited
//
//	@filename:
//		CHint.h
//
//	@doc:
//		Hint configurations
//---------------------------------------------------------------------------
#ifndef GPOPT_CHint_H
#define GPOPT_CHint_H

#include "gpos/base.h"
#include "gpos/common/CRefCount.h"
#include "gpos/memory/CMemoryPool.h"

#define JOIN_ORDER_DP_THRESHOLD ULONG(10)
#define BROADCAST_THRESHOLD ULONG(10000000)
#define PUSH_GROUP_BY_BELOW_SETOP_THRESHOLD ULONG(10)

/* POLAR px */
#define MAX_INSERT_DOP_NUM ULONG(128)
#define MAX_UPDATE_DOP_NUM ULONG(128)
#define MAX_SELECT_DOP_NUM ULONG(128)
#define MAX_DELETE_DOP_NUM ULONG(128)
/* POLAR px */

namespace gpopt
{
using namespace gpos;

//---------------------------------------------------------------------------
//	@class:
//		CHint
//
//	@doc:
//		Hint configurations
//
//---------------------------------------------------------------------------
class CHint : public CRefCount
{
private:
	ULONG m_ulMinNumOfPartsToRequireSortOnInsert;

	ULONG m_ulJoinArityForAssociativityCommutativity;

	ULONG m_ulArrayExpansionThreshold;

	ULONG m_ulJoinOrderDPLimit;

	ULONG m_ulBroadcastThreshold;

	BOOL m_fEnforceConstraintsOnDML;

	ULONG m_ulPushGroupByBelowSetopThreshold;

	/* POLAR px */
	ULONG m_ulInsertDopNum;

	ULONG m_ulUpdateDopNum;

	ULONG m_ulSelectDopNum;

	ULONG m_ulDeleteDopNum;

	BOOL m_fRemoveUpdateRedundantMotion;

	BOOL m_fRemoveDeleteRedundantMotion;
	/* POLAR px */

public:
	CHint(const CHint &) = delete;

	// ctor
	CHint(ULONG min_num_of_parts_to_require_sort_on_insert,
		  ULONG join_arity_for_associativity_commutativity,
		  ULONG array_expansion_threshold, ULONG ulJoinOrderDPLimit,
		  ULONG broadcast_threshold, BOOL enforce_constraint_on_dml,
		  ULONG push_group_by_below_setop_threshold,
		  /* POLAR px */
		  ULONG insert_dop_num,
		  ULONG update_dop_num,
		  ULONG select_dop_num,
		  ULONG delete_dop_num,
		  BOOL	remove_update_redundant_motion,
		  BOOL	remove_delete_redundant_motion)
		: m_ulMinNumOfPartsToRequireSortOnInsert(
			  min_num_of_parts_to_require_sort_on_insert),
		  m_ulJoinArityForAssociativityCommutativity(
			  join_arity_for_associativity_commutativity),
		  m_ulArrayExpansionThreshold(array_expansion_threshold),
		  m_ulJoinOrderDPLimit(ulJoinOrderDPLimit),
		  m_ulBroadcastThreshold(broadcast_threshold),
		  m_fEnforceConstraintsOnDML(enforce_constraint_on_dml),
		  m_ulPushGroupByBelowSetopThreshold(
			  push_group_by_below_setop_threshold),
			/* POLAR px */
		  m_ulInsertDopNum(insert_dop_num),
		  m_ulUpdateDopNum(update_dop_num),
		  m_ulSelectDopNum(select_dop_num),
		  m_ulDeleteDopNum(delete_dop_num),
		  m_fRemoveUpdateRedundantMotion(remove_update_redundant_motion),
		  m_fRemoveDeleteRedundantMotion(remove_delete_redundant_motion)
	{
	}

	// Minimum number of partitions required for sorting tuples during
	// insertion in an append only row-oriented partitioned table
	ULONG
	UlMinNumOfPartsToRequireSortOnInsert() const
	{
		return m_ulMinNumOfPartsToRequireSortOnInsert;
	}

	// Maximum number of relations in an n-ary join operator where ORCA will
	// explore JoinAssociativity and JoinCommutativity transformations.
	// When the number of relations exceed this we'll prune the search space
	// by not pursuing the above mentioned two transformations.
	ULONG
	UlJoinArityForAssociativityCommutativity() const
	{
		return m_ulJoinArityForAssociativityCommutativity;
	}

	// Maximum number of elements in the scalar comparison with an array which
	// will be expanded for constraint derivation. The benefits of using a smaller number
	// are avoiding expensive expansion of constraints in terms of memory and optimization
	// time. This is used to restrict constructs of following types when the constant-array
	// size is greater than threshold:
	// "(expression) scalar op ANY/ALL (array of constants)" OR
	// "(expression1, expression2) scalar op ANY/ALL ((const-x1, const-y1), ... (const-xn, const-yn))"
	ULONG
	UlArrayExpansionThreshold() const
	{
		return m_ulArrayExpansionThreshold;
	}

	// Maximum number of relations in an n-ary join operator where ORCA will
	// explore join ordering via dynamic programming.
	ULONG
	UlJoinOrderDPLimit() const
	{
		return m_ulJoinOrderDPLimit;
	}

	// Maximum number of rows ORCA will broadcast
	ULONG
	UlBroadcastThreshold() const
	{
		return m_ulBroadcastThreshold;
	}

	// If true, ORCA will add Assertion nodes to the plan to enforce CHECK
	// and NOT NULL constraints on inserted/updated values. (Otherwise it
	// is up to the executor to enforce them.)
	BOOL
	FEnforceConstraintsOnDML() const
	{
		return m_fEnforceConstraintsOnDML;
	}

	// Skip CXformPushGbBelowSetOp if set op arity is greater than this
	ULONG
	UlPushGroupByBelowSetopThreshold() const
	{
		return m_ulPushGroupByBelowSetopThreshold;
	}

	/* POLAR px */
	ULONG
	UlInsertDopNum() const
	{
		return m_ulInsertDopNum;
	}

	ULONG
	UlUpdateDopNum() const
	{
		return m_ulUpdateDopNum;
	}

	ULONG
	UlSelectDopNum() const
	{
		return m_ulSelectDopNum;
	}

	ULONG
	UlDeleteDopNum() const
	{
		return m_ulDeleteDopNum;
	}

	BOOL
	FRemoveUpdateRedundantMotion() const
	{
		return m_fRemoveUpdateRedundantMotion;
	}

	BOOL
	FRemoveDeleteRedundantMotion() const
	{
		return m_fRemoveDeleteRedundantMotion;
	}
	/* POLAR px */

	// generate default hint configurations, which disables sort during insert on
	// append only row-oriented partitioned tables by default
	static CHint *
	PhintDefault(CMemoryPool *mp)
	{
		return GPOS_NEW(mp) CHint(
			gpos::int_max, /* min_num_of_parts_to_require_sort_on_insert */
			gpos::int_max, /* join_arity_for_associativity_commutativity */
			gpos::int_max, /* array_expansion_threshold */
			JOIN_ORDER_DP_THRESHOLD,			/*ulJoinOrderDPLimit*/
			BROADCAST_THRESHOLD,				/*broadcast_threshold*/
			true,								/* enforce_constraint_on_dml */
			PUSH_GROUP_BY_BELOW_SETOP_THRESHOLD, /* push_group_by_below_setop_threshold */
			/* POLAR px */
			MAX_INSERT_DOP_NUM,
			MAX_UPDATE_DOP_NUM,
			MAX_SELECT_DOP_NUM,
			MAX_DELETE_DOP_NUM,
			true,
			true
			/* POLAR px */
		);
	}

};	// class CHint
}  // namespace gpopt

#endif	// !GPOPT_CHint_H

// EOF
