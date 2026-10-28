
#include "TimeTrial.h"

// Name: TimeTrial
// Description: Creates the initial time trial course
// Parameters: None
// Returns: None
TimeTrial::TimeTrial()
{
    // Create four checkpoints along the flight path
    checkpoints_.emplace_back(
        Vector3{ 0.0f, 5.0f, -20.0f }, 3.0f
    );

    checkpoints_.emplace_back(
        Vector3{ 4.0f, 6.0f, -45.0f }, 3.0f
    );

    checkpoints_.emplace_back(
        Vector3{ -3.0f, 8.0f, -70.0f }, 3.0f
    );

    checkpoints_.emplace_back(
        Vector3{ 2.0f, 5.0f, -95.0f }, 3.0f
    );
}

// Name: Update
// Description: Updates timer and checkpoint progression
// Parameters: deltaTime, previousPosition, currentPosition
// Returns: None
void TimeTrial::Update(
    float deltaTime,
    Vector3 previousPosition,
    Vector3 currentPosition)
{
    // Stop updating once the trial finishes
    if (finished_)
    {
        return;
    }

    // Update elapsed time
    elapsedTime_ += deltaTime;

    // Check only the next required checkpoint
    if (currentCheckpoint_ < checkpoints_.size())
    {
        const Checkpoint& checkpoint =
            checkpoints_[currentCheckpoint_];

        if (checkpoint.CheckCrossing(
            previousPosition,
            currentPosition))
        {
            currentCheckpoint_++;
        }
    }

    // Determine whether all checkpoints are complete
    if (currentCheckpoint_ >= checkpoints_.size())
    {
        finished_ = true;
    }
}

// Name: Draw
// Description: Draws remaining checkpoint rings
// Parameters: None
// Returns: None
void TimeTrial::Draw() const
{
    for (std::size_t i = currentCheckpoint_;
         i < checkpoints_.size();
         i++)
    {
        checkpoints_[i].Draw();
    }
}

// Name: Reset
// Description: Restarts the time trial
// Parameters: None
// Returns: None
void TimeTrial::Reset()
{
    currentCheckpoint_ = 0;
    elapsedTime_ = 0.0f;
    finished_ = false;
}

// Name: GetElapsedTime
// Description: Returns the time trial duration
// Parameters: None
// Returns: Elapsed time in seconds
float TimeTrial::GetElapsedTime() const
{
    return elapsedTime_;
}

// Name: GetPassedCount
// Description: Returns completed checkpoint count
// Parameters: None
// Returns: Number of completed checkpoints
std::size_t TimeTrial::GetPassedCount() const
{
    return currentCheckpoint_;
}

// Name: GetTotalCount
// Description: Returns total checkpoint count
// Parameters: None
// Returns: Number of checkpoints
std::size_t TimeTrial::GetTotalCount() const
{
    return checkpoints_.size();
}

// Name: IsFinished
// Description: Reports whether the trial is complete
// Parameters: None
// Returns: True when all checkpoints are passed
bool TimeTrial::IsFinished() const
{
    return finished_;
}
