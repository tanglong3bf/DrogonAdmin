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
    // 验证
    if (dept.name == newName)
    {
        // 新旧名称相同，无需更新
        co_return;
    }
    co_await deptVerifier_->verifyDeptNameNotDuplicated(newName, dept.parentId);

    // 更新值
    dept.name = newName;
    dept.markUpdatedBy(updatedBy);
    dept.markUpdated();
}

Task<> DeptHandler::deleteDept(Dept &dept, const int32_t deletedBy) const
{
    // 校验
    co_await deptVerifier_->verifyNoSubDept(*dept.deptId);
    co_await userVerifier_->verifyNoUserInDept(*dept.deptId);
    co_await roleVerifier_->verifyNoRolesBelongToDept(*dept.deptId);

    // 删除
    dept.markDeletedBy(deletedBy);
    dept.markDeleted();
}

Task<vector<Dept>> DeptHandler::sortDept(const optional<std::int32_t> &parentId,
                                         const vector<int32_t> &deptIds,
                                         const int32_t updatedBy) const
{
    const auto allDepts = co_await deptRepository_->getByParentId(parentId);
    if (parentId)
    {
        const auto parent = co_await deptRepository_->getById(*parentId);
        if (!parent)
        {
            throw BusinessException{"父部门不存在"};
        }
    }
    // 验证
    validateDeptIdsInAllDepts(deptIds, allDepts);

    // 部门id到allDepts索引的映射
    unordered_map<int32_t, size_t> idToIndex;
    for (size_t i = 0; i < allDepts.size(); ++i)
    {
        auto id = *allDepts[i].deptId;
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
            if (newSortNum != allDepts[idx].sortNum)
            {
                Dept dept = allDepts[idx];
                dept.sortNum = newSortNum;
                dept.markUpdatedBy(updatedBy);
                dept.markUpdated();
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

            if (newSortNum != allDepts[i].sortNum)
            {
                Dept dept = allDepts[i];
                dept.sortNum = newSortNum;
                dept.markUpdatedBy(updatedBy);
                dept.markUpdated();
                sortResult.push_back(std::move(dept));
            }
        }
    }
    co_return sortResult;
}

void DeptHandler::validateDeptIdsInAllDepts(const vector<int32_t> &deptIds,
                                            const vector<Dept> &allDepts) const
{
    unordered_set<int32_t> allDeptIds;
    for (const auto &dept : allDepts)
    {
        allDeptIds.insert(*dept.deptId);
    }

    for (const auto &deptId : deptIds)
    {
        if (allDeptIds.count(deptId) == 0)
        {
            throw BusinessException{"部分部门不是指定部门的子部门"};
        }
    }
}
