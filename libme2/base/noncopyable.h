#ifndef libmevent_BASE_NONCOPYABLE_H
#define libmevent_BASE_NONCOPYABLE_H

namespace libmevent
{

class noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

}  // namespace libmevent

#endif  // libmevent_BASE_NONCOPYABLE_H
