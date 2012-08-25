#include "document_lock.hpp"

#include <klosscreator/document.h>

namespace kloss {
namespace creator {

document_lock::document_lock(Document* document)
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
        LockDocument(document_);
        locked_ = true;
    }
}

void document_lock::unlock() noexcept
{
    if (locked_)
    {
        UnlockDocument(document_);
        locked_ = false;
    }
}

} // namespace creator
} // namespace kloss
