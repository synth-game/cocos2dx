/****************************************************************************
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/


#include "CCComponentContainer.h"
#include "CCComponent.h"
#include "CCDirector.h"

NS_CC_BEGIN

ComponentContainer::ComponentContainer(Node *node)
: _components()
, _owner(node)
{
}

ComponentContainer::~ComponentContainer(void)
{
}

Component* ComponentContainer::get(const char* name) const
{
    Component* ret = nullptr;
    do {
        CC_BREAK_IF(_components.empty());
		auto iter = _components.find(std::string(name));
		if(iter != _components.end()) {
			ret = iter->second;
		}
        
    } while (0);
    return ret;
}

bool ComponentContainer::add(Component *com)
{
    bool ret = false;
    CCASSERT(com != nullptr, "Argument must be non-nil");
    CCASSERT(com->getOwner() == nullptr, "Component already added. It can't be added again");
    do
    {
        if (_components.empty())
        {
            _owner->scheduleUpdate();
        }
        Component *component = nullptr;
		auto iter = _components.find(std::string(com->getName()));
		if(iter != _components.end()) {
			component = iter->second;
		}
        
        CCASSERT(component == nullptr, "Component already added. It can't be added again");
        CC_BREAK_IF(component);
        com->setOwner(_owner);
		_components.insert(std::make_pair(std::string(com->getName()), com));
		com->retain();
        com->onEnter();
        ret = true;
    } while(0);
    return ret;
}

bool ComponentContainer::remove(const std::string& name)
{
    bool ret = false;
    do 
    {        
        CC_BREAK_IF(_components.empty());
        
        auto iter = _components.find(name);
        CC_BREAK_IF(iter == _components.end());
        
        auto com = iter->second;
        com->onExit();
        com->setOwner(nullptr);
		com->release();

        _components.erase(iter);
        
        ret = true;
    } while(0);
    return ret;
 }

void ComponentContainer::removeAll()
{
    if (!_components.empty())
    {
        for (auto iter = _components.begin(); iter != _components.end(); ++iter)
        {
            iter->second->onExit();
            iter->second->setOwner(nullptr);
			iter->second->release();
        }
        
        _components.clear();
        
        _owner->unscheduleUpdate();
    }
}

void ComponentContainer::alloc(void)
{
    // do nothing, the map isn't a pointer
}

void ComponentContainer::visit(float delta)
{
	if (!_components.empty())
    {
        CC_SAFE_RETAIN(_owner);
        for (auto iter = _components.begin(); iter != _components.end(); ++iter)
        {
			Component* component = iter->second; 
            component->update(delta);
        }
        CC_SAFE_RELEASE(_owner);
    }
}

bool ComponentContainer::isEmpty() const
{
    return (_components.empty());
}

NS_CC_END