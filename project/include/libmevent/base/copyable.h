#ifndef BASE_COPYABLE_H
#define BASE_COPYABLE_H
namespace libmevent
{

/// A tag class emphasises the objects are copyable.
/// The empty base class optimization applies.
/// Any derived class of copyable should be a value type.
class copyable
{
protected:
    copyable() = default;
    ~copyable() = default;
};
}
#endif  // libmevent_BASE_COPYABLE_H
