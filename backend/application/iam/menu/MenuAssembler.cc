#include "MenuAssembler.h"

#include "domain/iam/menu/Menu.h"

using namespace std;
using namespace drogon;

Task<Menu> MenuAssembler::fromCreateRequest(const MenuCreateRequest &request,
                                            std::int32_t createdBy)
{
    // 验证
    if (request.parentId())
    {
        co_await menuVerifier_->verifyMenuIsDirectory(*request.parentId());
    }
    co_await menuVerifier_->verifyMenuNameNotDuplicated(string(request.name()),
                                                        request.parentId());

    // 准备必备参数
    const auto maxSortNum =
        co_await menuRepository_->getMaxSubMenuSortNum(request.parentId());
    const int32_t nextSortNum = maxSortNum ? *maxSortNum + 1 : 0;

    Menu menu{request.name(), nextSortNum, request.type(), createdBy};

    // 设置可选字段
    if (request.parentId())
    {
        menu.setParentId(*request.parentId());
    }

    switch (request.type())
    {
        case MenuType::Directory:
            menu.constructOptionalFields(request.icon(),
                                         std::nullopt,
                                         std::nullopt);
            break;

        case MenuType::Page:
            menu.constructOptionalFields(request.icon(),
                                         request.path(),
                                         request.component());
            break;

        case MenuType::OutLink:
            menu.constructOptionalFields(request.icon(),
                                         request.path(),
                                         std::nullopt);
            break;
    }

    menu.markNew();
    co_return menu;
}
