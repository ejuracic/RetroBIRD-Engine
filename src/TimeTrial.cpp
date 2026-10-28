#include "TimeTrial.h"

namespace
{
    constexpr float kGoldTime = 22.0f;
    constexpr float kSilverTime = 30.0f;
    constexpr float kBronzeTime = 45.0f;
}

// Name: TimeTrial
// Description: Creates the initial time trial course
// Parameters: None
// Returns: None
TimeTrial::TimeTrial()
{
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
// Description: Updates timing and checkpoint progression
// Parameters: deltaTime, previousPosition, currentPosition
// Returns: None
void TimeTrial::Update(
    float deltaTime,
    Vector3 previousPosition,
    Vector3 currentPosition)
{
    if (finished_)
    {
        return;
    }

    // Update the race timer
    elapsedTime_ += deltaTime;

    // Check the next required checkpoint
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

    // Check whether the race is complete
    if (currentCheckpoint_ >= checkpoints_.size())
    {
        finished_ = true;

        // Record a new personal best
        if (bestTime_ < 0.0f ||
            elapsedTime_ < bestTime_)
        {
            bestTime_ = elapsedTime_;
        }
    }
}

// Name: Draw
// Description: Draws the remaining checkpoints
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
// Description: Resets the current attempt
// Parameters: None
// Returns: None
void TimeTrial::Reset()
{
    currentCheckpoint_ = 0;
    elapsedTime_ = 0.0f;
    finished_ = false;

    // Best time is intentionally preserved
}

// Name: GetElapsedTime
// Description: Returns the current race time
// Parameters: None
// Returns: Elapsed seconds
float TimeTrial::GetElapsedTime() const
{
    return elapsedTime_;
}

// Name: GetBestTime
// Description: Returns the fastest completed race time
// Parameters: None
// Returns: Best time in seconds or -1
float TimeTrial::GetBestTime() const
{
    return bestTime_;
}

// Name: HasBestTime
// Description: Checks whether a best time exists
// Parameters: None
// Returns: True when a completed time is recorded
bool TimeTrial::HasBestTime() const
{
    return bestTime_ >= 0.0f;
}

// Name: IsFinished
// Description: Checks whether the race is complete
// Parameters: None
// Returns: True when all checkpoints are passed
bool TimeTrial::IsFinished() const
{
    return finished_;
}

// Name: GetPassedCount
// Description: Returns completed checkpoint count
// Parameters: None
// Returns: Completed checkpoints
std::size_t TimeTrial::GetPassedCount() const
{
    return currentCheckpoint_;
}

// Name: GetTotalCount
// Description: Returns total checkpoint count
// Parameters: None
// Returns: Total checkpoints
std::size_t TimeTrial::GetTotalCount() const
{
    return checkpoints_.size();
}

// Name: GetMedal
// Description: Determines the medal earned for a race
// Parameters: None
// Returns: TrialMedal
TrialMedal TimeTrial::GetMedal() const
{
    if (!finished_)
    {
        return TrialMedal::None;
    }

    if (elapsedTime_ <= kGoldTime)
    {
        return TrialMedal::Gold;
    }

    if (elapsedTime_ <= kSilverTime)
    {
        return TrialMedal::Silver;
    }

    if (elapsedTime_ <= kBronzeTime)
    {
        return TrialMedal::Bronze;
    }

    return TrialMedal::None;
}
