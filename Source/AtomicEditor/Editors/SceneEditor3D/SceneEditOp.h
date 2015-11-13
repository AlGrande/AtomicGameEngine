//
// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// LICENSE: Atomic Game Engine Editor and Tools EULA
// Please see LICENSE_ATOMIC_EDITOR_AND_TOOLS.md in repository root for
// license information: https://github.com/AtomicGameEngine/AtomicGameEngine
//

#pragma once

#include <Atomic/Core/Object.h>
#include <Atomic/IO/VectorBuffer.h>

namespace Atomic
{
class Node;
class Component;
}

using namespace Atomic;

namespace AtomicEditor
{

enum SceneEditType
{
    SCENEEDIT_UNKNOWN = 0,
    SCENEEDIT_SELECTION
};

class SceneEditOp
{

public:

    SceneEditOp(SceneEditType type) { type_ = type; }
    virtual ~SceneEditOp() { }

    virtual bool Undo() = 0;
    virtual bool Redo() = 0;

    SceneEditType type_;

};

class SelectionEditOp : public SceneEditOp
{

public:

    SelectionEditOp(Vector<SharedPtr<Node>>& nodes);
    ~SelectionEditOp();

    bool Undo();
    bool Redo();

    void RegisterEdit();

    bool Commit();

private:

    struct EditComponent
    {
        SharedPtr<Component> component_;
        SharedPtr<Node> nodeBegin_;
        SharedPtr<Node> nodeEnd_;
        VectorBuffer stateBegin_;
        VectorBuffer stateEnd_;
    };

    struct EditNode
    {
        SharedPtr<Node> node_;
        SharedPtr<Node> parentBegin_;
        SharedPtr<Node> parentEnd_;
        VectorBuffer stateBegin_;
        VectorBuffer stateEnd_;
        PODVector<EditComponent*> components_;
    };

    PODVector<EditNode*> editNodes_;

};

}

