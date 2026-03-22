#include "DeptHandler.h"
#include <unordered_set>

#include "common/exception/BusinessException.h"

using namespace std;
using namespace drogon;
using namespace drogon::orm;

Task<> DeptHandler::updateDept(Dept &dept,
                               const string &newName,
                               const int32_t updatedBy) const
{
    validateNameNotSame(dept.getName(), newName);
    co_await deptVerifier_->verifyDeptNameNotDuplicated(newName,
                                                        dept.getParentId());

    dept.setName(newName);
    dept.setUpdatedBy(updatedBy);
}

drogon::Task<> DeptHandler::deleteDept(Dept &dept,
                                       const int32_t deletedBy) const
{
    co_await deptVerifier_->verifyNoSubDept(*dept.getDeptId());
    co_await userVerifier_->verifyNoUserInDept(*dept.getDeptId());
    co_await roleVerifier_->ensureNoRolesBelongToDept(*dept.getDeptId());

    dept.setDeletedBy(deletedBy);
}

drogon::Task<vector<Dept>> DeptHandler::sortDept(const vector<int32_t> &deptIds,
                                                 const vector<Dept> &allDepts,
                                                 const int32_t updatedBy) const
{
    validateDeptIdsInAllDepts(deptIds, allDepts);

    // 部门id到allDepts索引的映射
    unordered_map<int32_t, size_t> idToIndex;
    for (size_t i = 0; i < allDepts.size(); ++i)
    {
        auto id = *allDepts[i].getDeptId();
        idToIndex[id] = i;
    }

    // 哪些索引的部门已经被处理
    vector<bool> processed(allDepts.size(), false);

    // 排序结果
    vector<Dept> sortResult;
    sortResult.reserve(allDepts.size());

    size_t deptProcessedCount = 0;

    // 先处理dpetIds指定的部门
    for (int32_t id : deptIds)
    {
        const auto it = idToIndex.find(id);
        if (it != idToIndex.end())
        {
            const int32_t newSortNum = deptProcessedCount++;
            const size_t idx = it->second;
            if (newSortNum != allDepts[idx].getSortNum())
            {
                Dept dept = allDepts[idx];
                dept.setSortNum(newSortNum);
                dept.setUpdatedBy(updatedBy);
                sortResult.push_back(std::move(dept));
            }
            processed[idx] = true;
        }
    }

    // 再处理剩下的部门，保持原有顺序
    for (size_t i = 0; i < allDepts.size(); ++i)
    {
        if (!processed[i])
        {
            const int32_t newSortNum = deptProcessedCount++;

            if (newSortNum != allDepts[i].getSortNum())
            {
                Dept dept = allDepts[i];
                dept.setSortNum(newSortNum);
                dept.setUpdatedBy(updatedBy);
                sortResult.push_back(std::move(dept));
            }
        }
    }
    co_return sortResult;
}

void DeptHandler::validateNameNotSame(const string &oldName,
                                      const string &newName) const
{
    if (oldName == newName)
    {
        throw BusinessException{"部门新名称和旧名称相同"};
    }
}

void DeptHandler::validateDeptIdsInAllDepts(const vector<int32_t> &deptIds,
                                            const vector<Dept> &allDepts) const
{
    unordered_set<int32_t> allDeptIds;
    for (const auto &dept : allDepts)
    {
        allDeptIds.insert(*dept.getDeptId());
    }

    for (const auto &deptId : deptIds)
    {
        if (allDeptIds.count(deptId) == 0)
        {
            throw BusinessException{"部门ID " + to_string(deptId) +
                                    " 不是指定部门的子部门"};
        }
    }
}
