/*
 * Copyright (c) 2007, Michael Feathers, James Grenning and Bas Vodde
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE EARLIER MENTIONED AUTHORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockNamedValue.h"
#include "CppUTest/PlatformSpecificFunctions.h"


MockNamedValueComparatorsAndCopiersRepository* MockNamedValueBase::defaultRepository_ = NULLPTR;

void MockNamedValue::setDefaultComparatorsAndCopiersRepository(MockNamedValueComparatorsAndCopiersRepository* repository)
{
    defaultRepository_ = repository;
}

MockNamedValueBase::MockNamedValueBase(const SimpleString& name) : name_(name), type_("int"), size_(0), comparator_(NULLPTR), copier_(NULLPTR)
{
    value_.intValue_ = 0;
}

MockNamedValueBase::~MockNamedValueBase()
{
}

void MockNamedValueBase::setValue(bool value)
{
    type_ = "bool";
    value_.boolValue_ = value;
}

void MockNamedValueBase::setValue(unsigned int value)
{
    type_ = "unsigned int";
    value_.unsignedIntValue_ = value;
}

void MockNamedValueBase::setValue(int value)
{
    type_ = "int";
    value_.intValue_ = value;
}

void MockNamedValueBase::setValue(long int value)
{
    type_ = "long int";
    value_.longIntValue_ = value;
}

void MockNamedValueBase::setValue(unsigned long int value)
{
    type_ = "unsigned long int";
    value_.unsignedLongIntValue_ = value;
}

void MockNamedValueBase::setValue(double value)
{
    type_ = "double";
    value_.doubleValue_ = value;
}

void MockNamedValueBase::setValue(void* value)
{
    type_ = "void*";
    value_.pointerValue_ = value;
}

void MockNamedValueBase::setValue(const void* value)
{
    type_ = "const void*";
    value_.constPointerValue_ = value;
}

void MockNamedValueBase::setValue(void (*value)())
{
    type_ = "void (*)()";
    value_.functionPointerValue_ = value;
}

void MockNamedValueBase::setValue(const char* value)
{
    type_ = "const char*";
    value_.stringValue_ = value;
}

void MockNamedValueBase::setMemoryBuffer(const unsigned char* value, size_t size)
{
    type_ = "const unsigned char*";
    value_.memoryBufferValue_ = value;
    size_ = size;
}

void MockNamedValueBase::setConstObjectPointer(const SimpleString& type, const void* objectPtr)
{
    type_ = type;
    value_.constObjectPointerValue_ = objectPtr;
    if (defaultRepository_)
    {
        comparator_ = defaultRepository_->getComparatorForType(type);
        copier_ = defaultRepository_->getCopierForType(type);
    }
}

void MockNamedValueBase::setObjectPointer(const SimpleString& type, void* objectPtr)
{
    type_ = type;
    value_.objectPointerValue_ = objectPtr;
    if (defaultRepository_)
    {
        comparator_ = defaultRepository_->getComparatorForType(type);
        copier_ = defaultRepository_->getCopierForType(type);
    }
}

void MockNamedValueBase::setSize(size_t size)
{
    size_ = size;
}

void MockNamedValueBase::setName(const char* name)
{
    name_ = name;
}

SimpleString MockNamedValueBase::getName() const
{
    return name_;
}

SimpleString MockNamedValueBase::getType() const
{
    return type_;
}

bool MockNamedValueBase::getBoolValue() const
{
    STRCMP_EQUAL("bool", type_.asCharString());
    return value_.boolValue_;
}

unsigned int MockNamedValueBase::getUnsignedIntValue() const
{
    if(type_ == "int" && value_.intValue_ >= 0)
        return (unsigned int)value_.intValue_;
    else
    {
        STRCMP_EQUAL("unsigned int", type_.asCharString());
        return value_.unsignedIntValue_;
    }
}

int MockNamedValueBase::getIntValue() const
{
    STRCMP_EQUAL("int", type_.asCharString());
    return value_.intValue_;
}

long int MockNamedValueBase::getLongIntValue() const
{
    if(type_ == "int")
        return value_.intValue_;
    else if(type_ == "unsigned int")
        return (long int)value_.unsignedIntValue_;
    else
    {
        STRCMP_EQUAL("long int", type_.asCharString());
        return value_.longIntValue_;
    }
}

unsigned long int MockNamedValueBase::getUnsignedLongIntValue() const
{
    if(type_ == "unsigned int")
        return value_.unsignedIntValue_;
    else if(type_ == "int" && value_.intValue_ >= 0)
        return (unsigned long int)value_.intValue_;
    else if(type_ == "long int" && value_.longIntValue_ >= 0)
        return (unsigned long int)value_.longIntValue_;
    else
    {
        STRCMP_EQUAL("unsigned long int", type_.asCharString());
        return value_.unsignedLongIntValue_;
    }
}

double MockNamedValueBase::getDoubleValue() const
{
    STRCMP_EQUAL("double", type_.asCharString());
    return value_.doubleValue_;
}

const char* MockNamedValueBase::getStringValue() const
{
    STRCMP_EQUAL("const char*", type_.asCharString());
    return value_.stringValue_;
}

void* MockNamedValueBase::getPointerValue() const
{
    STRCMP_EQUAL("void*", type_.asCharString());
    return value_.pointerValue_;
}

const void* MockNamedValueBase::getConstPointerValue() const
{
    STRCMP_EQUAL("const void*", type_.asCharString());
    return value_.pointerValue_;
}

void (*MockNamedValueBase::getFunctionPointerValue() const)()
{
    STRCMP_EQUAL("void (*)()", type_.asCharString());
    return value_.functionPointerValue_;
}

const unsigned char* MockNamedValueBase::getMemoryBuffer() const
{
    STRCMP_EQUAL("const unsigned char*", type_.asCharString());
    return value_.memoryBufferValue_;
}

const void* MockNamedValueBase::getConstObjectPointer() const
{
    return value_.constObjectPointerValue_;
}

void* MockNamedValueBase::getObjectPointer() const
{
    return value_.objectPointerValue_;
}

size_t MockNamedValueBase::getSize() const
{
    return size_;
}

MockNamedValueComparator* MockNamedValueBase::getComparator() const
{
    return comparator_;
}

MockNamedValueCopier* MockNamedValueBase::getCopier() const
{
    return copier_;
}

bool MockNamedValueBase::equals(const MockNamedValue& p) const
{
    if((type_ == "long int") && (p.type_ == "int"))
        return value_.longIntValue_ == p.value_.intValue_;
    else if((type_ == "int") && (p.type_ == "long int"))
        return value_.intValue_ == p.value_.longIntValue_;
    else if((type_ == "unsigned int") && (p.type_ == "int"))
        return (p.value_.intValue_ >= 0) && (value_.unsignedIntValue_ == (unsigned int)p.value_.intValue_);
    else if((type_ == "int") && (p.type_ == "unsigned int"))
        return (value_.intValue_ >= 0) && ((unsigned int)value_.intValue_ == p.value_.unsignedIntValue_);
    else if((type_ == "unsigned long int") && (p.type_ == "int"))
        return (p.value_.intValue_ >= 0) && (value_.unsignedLongIntValue_ == (unsigned long)p.value_.intValue_);
    else if((type_ == "int") && (p.type_ == "unsigned long int"))
        return (value_.intValue_ >= 0) && ((unsigned long)value_.intValue_ == p.value_.unsignedLongIntValue_);
    else if((type_ == "unsigned int") && (p.type_ == "long int"))
        return (p.value_.longIntValue_ >= 0) && (value_.unsignedIntValue_ == (unsigned long)p.value_.longIntValue_);
    else if((type_ == "long int") && (p.type_ == "unsigned int"))
        return (value_.longIntValue_ >= 0) && ((unsigned long)value_.longIntValue_ == p.value_.unsignedIntValue_);
    else if((type_ == "unsigned int") && (p.type_ == "unsigned long int"))
        return value_.unsignedIntValue_ == p.value_.unsignedLongIntValue_;
    else if((type_ == "unsigned long int") && (p.type_ == "unsigned int"))
        return value_.unsignedLongIntValue_ == p.value_.unsignedIntValue_;
    else if((type_ == "long int") && (p.type_ == "unsigned long int"))
        return (value_.longIntValue_ >= 0) && ((unsigned long)value_.longIntValue_ == p.value_.unsignedLongIntValue_);
    else if((type_ == "unsigned long int") && (p.type_ == "long int"))
        return (p.value_.longIntValue_ >= 0) && (value_.unsignedLongIntValue_ == (unsigned long) p.value_.longIntValue_);
#if CPPUTEST_USE_LONG_LONG == 1
    else if ((type_ == "long long int") && (p.type_ == "int"))
        return value_.longLongIntValue_ == p.value_.intValue_;
    else if ((type_ == "int") && (p.type_ == "long long int"))
        return value_.intValue_ == p.value_.longLongIntValue_;
    else if ((type_ == "long long int") && (p.type_ == "long int"))
        return value_.longLongIntValue_ == p.value_.longIntValue_;
    else if ((type_ == "long int") && (p.type_ == "long long int"))
        return value_.longIntValue_ == p.value_.longLongIntValue_;
    else if ((type_ == "long long int") && (p.type_ == "unsigned int"))
        return (value_.longLongIntValue_ >= 0) && ((unsigned long long)value_.longLongIntValue_ == p.value_.unsignedIntValue_);
    else if ((type_ == "unsigned int") && (p.type_ == "long long int"))
        return (p.value_.longLongIntValue_ >= 0) && (value_.unsignedIntValue_ == (unsigned long long)p.value_.longLongIntValue_);
    else if ((type_ == "long long int") && (p.type_ == "unsigned long int"))
        return (value_.longLongIntValue_ >= 0) && ((unsigned long long)value_.longLongIntValue_ == p.value_.unsignedLongIntValue_);
    else if ((type_ == "unsigned long int") && (p.type_ == "long long int"))
        return (p.value_.longLongIntValue_ >= 0) && (value_.unsignedLongIntValue_ == (unsigned long long)p.value_.longLongIntValue_);
    else if ((type_ == "long long int") && (p.type_ == "unsigned long long int"))
        return (value_.longLongIntValue_ >= 0) && ((unsigned long long)value_.longLongIntValue_ == p.value_.unsignedLongLongIntValue_);
    else if ((type_ == "unsigned long long int") && (p.type_ == "long long int"))
        return (p.value_.longLongIntValue_ >= 0) && (value_.unsignedLongLongIntValue_ == (unsigned long long)p.value_.longLongIntValue_);
    else if ((type_ == "unsigned long long int") && (p.type_ == "int"))
        return (p.value_.intValue_ >= 0) && (value_.unsignedLongLongIntValue_ == (unsigned long long)p.value_.intValue_);
    else if ((type_ == "int value") && (p.type_ == "unsigned long long int"))
        return (value_.intValue_ >= 0) && ((unsigned long long)value_.intValue_ == p.value_.unsignedLongLongIntValue_);
    else if ((type_ == "unsigned long long int") && (p.type_ == "unsigned int"))
        return value_.unsignedLongLongIntValue_ == p.value_.unsignedIntValue_;
    else if ((type_ == "unsigned int") && (p.type_ == "unsigned long long int"))
        return value_.unsignedIntValue_ == p.value_.unsignedLongLongIntValue_;
    else if ((type_ == "unsigned long long int") && (p.type_ == "long int"))
        return (p.value_.longIntValue_ >= 0) && (value_.unsignedLongLongIntValue_ == (unsigned long long)p.value_.longIntValue_);
    else if ((type_ == "long int") && (p.type_ == "unsigned long long int"))
        return (value_.longIntValue_ >= 0) && ((unsigned long long)value_.longLongIntValue_ == p.value_.unsignedLongLongIntValue_);
    else if ((type_ == "unsigned long long int") && (p.type_ == "unsigned long int"))
        return value_.unsignedLongLongIntValue_ == p.value_.unsignedLongIntValue_;
    else if ((type_ == "unsigned long int") && (p.type_ == "unsigned long long int"))
        return value_.unsignedLongIntValue_ == p.value_.unsignedLongLongIntValue_;
#endif

    if (type_ != p.type_) return false;

    if (type_ == "bool")
        return value_.boolValue_ == p.value_.boolValue_;
    else if (type_ == "int")
        return value_.intValue_ == p.value_.intValue_;
    else if (type_ == "unsigned int")
        return value_.unsignedIntValue_ == p.value_.unsignedIntValue_;
    else if (type_ == "long int")
        return value_.longIntValue_ == p.value_.longIntValue_;
    else if (type_ == "unsigned long int")
        return value_.unsignedLongIntValue_ == p.value_.unsignedLongIntValue_;
#if CPPUTEST_USE_LONG_LONG == 1
    else if (type_ == "long long int")
        return value_.longLongIntValue_ == p.value_.longLongIntValue_;
    else if (type_ == "unsigned long long int")
        return value_.unsignedLongLongIntValue_ == p.value_.unsignedLongLongIntValue_;
#endif
    else if (type_ == "const char*")
        return SimpleString(value_.stringValue_) == SimpleString(p.value_.stringValue_);
    else if (type_ == "void*")
        return value_.pointerValue_ == p.value_.pointerValue_;
    else if (type_ == "const void*")
        return value_.constPointerValue_ == p.value_.constPointerValue_;
    else if (type_ == "void (*)()")
        return value_.functionPointerValue_ == p.value_.functionPointerValue_;
    else if (type_ == "double")
        return (doubles_equal(value_.doubleValue_, p.value_.doubleValue_, 0.005));
    else if (type_ == "const unsigned char*")
    {
        if (size_ != p.size_) {
            return false;
        }
        return SimpleString::MemCmp(value_.memoryBufferValue_, p.value_.memoryBufferValue_, size_) == 0;
    }

    if (comparator_)
        return comparator_->isEqual(value_.constObjectPointerValue_, p.value_.constObjectPointerValue_);

    return false;
}

bool MockNamedValueBase::compatibleForCopying(const MockNamedValue& p) const
{
    if (type_ == p.type_) return true;

    if ((type_ == "const void*") && (p.type_ == "void*"))
        return true;

    return false;
}

SimpleString MockNamedValueBase::toString() const
{
    if (type_ == "bool")
        return StringFrom(value_.boolValue_);
    else if (type_ == "int")
        return StringFrom(value_.intValue_) + " " + BracketsFormattedHexStringFrom(value_.intValue_);
    else if (type_ == "unsigned int")
        return StringFrom(value_.unsignedIntValue_) + " " + BracketsFormattedHexStringFrom(value_.unsignedIntValue_);
    else if (type_ == "long int")
        return StringFrom(value_.longIntValue_) + " " + BracketsFormattedHexStringFrom(value_.longIntValue_);
    else if (type_ == "unsigned long int")
        return StringFrom(value_.unsignedLongIntValue_) + " " + BracketsFormattedHexStringFrom(value_.unsignedLongIntValue_);
#if CPPUTEST_USE_LONG_LONG == 1
    else if (type_ == "long long int")
        return StringFrom(value_.longLongIntValue_) + " " + BracketsFormattedHexStringFrom(value_.longLongIntValue_);
    else if (type_ == "unsigned long long int")
        return StringFrom(value_.unsignedLongLongIntValue_) + " " + BracketsFormattedHexStringFrom(value_.unsignedLongLongIntValue_);
#endif
    else if (type_ == "const char*")
        return value_.stringValue_;
    else if (type_ == "void*")
        return StringFrom(value_.pointerValue_);
    else if (type_ == "void (*)()")
        return StringFrom(value_.functionPointerValue_);
    else if (type_ == "const void*")
        return StringFrom(value_.constPointerValue_);
    else if (type_ == "double")
        return StringFrom(value_.doubleValue_);
    else if (type_ == "const unsigned char*")
        return StringFromBinaryWithSizeOrNull(value_.memoryBufferValue_, size_);

    if (comparator_)
        return comparator_->valueToString(value_.constObjectPointerValue_);

    return StringFromFormat("No comparator found for type: \"%s\"", type_.asCharString());

}

#if CPPUTEST_USE_LONG_LONG == 1

MockNamedValueExtended::MockNamedValueExtended(const SimpleString& name) : MockNamedValueBase(name)
{
}

MockNamedValueExtended::~MockNamedValueExtended()
{
}

void MockNamedValueExtended::setValue(long long int value)
{
    type_ = "long long int";
    value_.longLongIntValue_ = value;
}

void MockNamedValueExtended::setValue(unsigned long long int value)
{
    type_ = "unsigned long long int";
    value_.unsignedLongLongIntValue_ = value;
}

long long int MockNamedValueExtended::getLongLongIntValue() const
{
    if(type_ == "int")
        return value_.intValue_;
    else if(type_ == "unsigned int")
        return (long int)value_.unsignedIntValue_;
    else if(type_ == "long int")
        return (long long int)value_.longIntValue_;
    else if(type_ == "unsigned long int")
        return (long long int)value_.unsignedLongIntValue_;
    else
    {
        STRCMP_EQUAL("long long int", type_.asCharString());
        return value_.longIntValue_;
    }
}

unsigned long long int MockNamedValueExtended::getUnsignedLongLongIntValue() const
{
    if(type_ == "unsigned int")
        return value_.unsignedIntValue_;
    else if(type_ == "int" && value_.intValue_ >= 0)
        return (unsigned long long int)value_.intValue_;
    else if(type_ == "unsigned long int")
        return value_.unsignedLongIntValue_;
    else if(type_ == "long int" && value_.longIntValue_ >= 0)
        return (unsigned long long int)value_.longIntValue_;
    else if(type_ == "long long int" && value_.longLongIntValue_ >= 0)
        return (unsigned long long int)value_.longLongIntValue_;
    else
    {
        STRCMP_EQUAL("unsigned long long int", type_.asCharString());
        return value_.unsignedLongLongIntValue_;
    }
}

#endif

MockNamedValue::MockNamedValue(const SimpleString& name)
#if CPPUTEST_USE_LONG_LONG == 1
: MockNamedValueExtended(name)
#else
: MockNamedValueBase(name)
#endif
{
}

void MockNamedValueListNode::setNext(MockNamedValueListNode* node)
{
    next_ = node;
}

MockNamedValueListNode* MockNamedValueListNode::next()
{
    return next_;
}

MockNamedValue* MockNamedValueListNode::item()
{
    return data_;
}

void MockNamedValueListNode::destroy()
{
    delete data_;
}

MockNamedValueListNode::MockNamedValueListNode(MockNamedValue* newValue)
    : data_(newValue), next_(NULLPTR)
{
}

SimpleString MockNamedValueListNode::getName() const
{
    return data_->getName();
}

SimpleString MockNamedValueListNode::getType() const
{
    return data_->getType();
}

MockNamedValueList::MockNamedValueList() : head_(NULLPTR)
{
}

void MockNamedValueList::clear()
{
    while (head_) {
        MockNamedValueListNode* n = head_->next();
        head_->destroy();
        delete head_;
        head_ = n;
    }
}

void MockNamedValueList::add(MockNamedValue* newValue)
{
    MockNamedValueListNode* newNode = new MockNamedValueListNode(newValue);
    if (head_ == NULLPTR)
        head_ = newNode;
    else {
        MockNamedValueListNode* lastNode = head_;
        while (lastNode->next()) lastNode = lastNode->next();
        lastNode->setNext(newNode);
    }
}

MockNamedValue* MockNamedValueList::getValueByName(const SimpleString& name)
{
    for (MockNamedValueListNode * p = head_; p; p = p->next())
        if (p->getName() == name)
            return p->item();
    return NULLPTR;
}

MockNamedValueListNode* MockNamedValueList::begin()
{
    return head_;
}

struct MockNamedValueComparatorsAndCopiersRepositoryNode
{
    MockNamedValueComparatorsAndCopiersRepositoryNode(const SimpleString& name, MockNamedValueComparator* comparator, MockNamedValueComparatorsAndCopiersRepositoryNode* next)
        : name_(name), comparator_(comparator), copier_(NULLPTR), next_(next) {}
    MockNamedValueComparatorsAndCopiersRepositoryNode(const SimpleString& name, MockNamedValueCopier* copier, MockNamedValueComparatorsAndCopiersRepositoryNode* next)
        : name_(name), comparator_(NULLPTR), copier_(copier), next_(next) {}
    MockNamedValueComparatorsAndCopiersRepositoryNode(const SimpleString& name, MockNamedValueComparator* comparator, MockNamedValueCopier* copier, MockNamedValueComparatorsAndCopiersRepositoryNode* next)
        : name_(name), comparator_(comparator), copier_(copier), next_(next) {}
    SimpleString name_;
    MockNamedValueComparator* comparator_;
    MockNamedValueCopier* copier_;
    MockNamedValueComparatorsAndCopiersRepositoryNode* next_;
};

MockNamedValueComparatorsAndCopiersRepository::MockNamedValueComparatorsAndCopiersRepository() : head_(NULLPTR)
{

}

MockNamedValueComparatorsAndCopiersRepository::~MockNamedValueComparatorsAndCopiersRepository()
{
    clear();
}

void MockNamedValueComparatorsAndCopiersRepository::clear()
{
    while (head_) {
        MockNamedValueComparatorsAndCopiersRepositoryNode* next = head_->next_;
        delete head_;
        head_ = next;
    }
}

void MockNamedValueComparatorsAndCopiersRepository::installComparator(const SimpleString& name, MockNamedValueComparator& comparator)
{
    head_ = new MockNamedValueComparatorsAndCopiersRepositoryNode(name, &comparator, head_);
}

void MockNamedValueComparatorsAndCopiersRepository::installCopier(const SimpleString& name, MockNamedValueCopier& copier)
{
    head_ = new MockNamedValueComparatorsAndCopiersRepositoryNode(name, &copier, head_);
}

MockNamedValueComparator* MockNamedValueComparatorsAndCopiersRepository::getComparatorForType(const SimpleString& name)
{
    for (MockNamedValueComparatorsAndCopiersRepositoryNode* p = head_; p; p = p->next_)
            if (p->name_ == name && p->comparator_) return p->comparator_;
    return NULLPTR;
}

MockNamedValueCopier* MockNamedValueComparatorsAndCopiersRepository::getCopierForType(const SimpleString& name)
{
    for (MockNamedValueComparatorsAndCopiersRepositoryNode* p = head_; p; p = p->next_)
            if (p->name_ == name && p->copier_) return p->copier_;
    return NULLPTR;
}

void MockNamedValueComparatorsAndCopiersRepository::installComparatorsAndCopiers(const MockNamedValueComparatorsAndCopiersRepository& repository)
{
    for (MockNamedValueComparatorsAndCopiersRepositoryNode* p = repository.head_; p; p = p->next_)
      head_ = new MockNamedValueComparatorsAndCopiersRepositoryNode(p->name_, p->comparator_, p->copier_, head_);
}
