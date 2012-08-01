#include "document_lock.hpp"

#include <klosscreator/document.hpp>

namespace kloss {
namespace creator {

document_lock::document_lock(document& document)
    : document_(document)
    , locked_(false)
{
}

document_lock::~document_lock()
{
    unlock();
}

void document_lock::lock() noexcept
{
    if (!locked_)
    {
        document_.lock();
        locked_ = true;
    }
}

void document_lock::unlock() noexcept
{
    if (locked_)
    {
        document_.unlock();
        locked_ = false;
    }
}

} // namespace creator
} // namespace kloss
