#pragma once

#include <proc_managers/workers/Worker.h>
#include <helpers/FixedLinesFileWriter.h>

namespace mw { namespace ipc {

    class IIpc;

} } // mw::ipc

namespace mw { namespace proc_managers { namespace workers {

class PressureWorker : public Worker {
public:
    PressureWorker(ipc::IIpc& ipcData, const std::size_t bufferSize);
    ~PressureWorker() = default;

    PressureWorker(const PressureWorker&) = delete;
    PressureWorker& operator = (const PressureWorker&) = delete;
    PressureWorker(PressureWorker&&) = delete;
    PressureWorker& operator = (PressureWorker&&) = delete;

    void processData() override;

private:
    helpers::FixedLinesFileWriter writer;
};

} } } // mw::proc_managers::workers