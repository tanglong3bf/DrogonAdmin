#include "MenuService.h"

#include "common/util/rangesUtils.hpp"

using namespace std;
using namespace drogon;

Task<vector<MenuResponse>> MenuService::getMenuTree() const
{
    co_return co_await menuCqrsRepo_->getMenuTree();
}
