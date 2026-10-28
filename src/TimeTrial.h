#pragma once

#include "raylib.h"
#include "Checkpoint.h"

#include <cstddef>
#include <vector>

enum class TrialMedal
{
    None,
    Bronze,
    Silver,
    Gold
};

class TimeTrial
{
public:
    TimeTrial();

    void Update(
        float deltaTime,
        Vector3 previousPosition,
        Vector3 currentPosition
    );

    void Draw() const;
    void Reset();

    float GetElapsedTime() const;
    float GetBestTime() const;

    bool HasBestTime() const;
    bool IsFinished() const;

    std::size_t GetPassedCount() const;
    std::size_t GetTotalCount() const;

    TrialMedal GetMedal() const;

private:
    std::vector<Checkpoint> checkpoints_;

    std::size_t currentCheckpoint_ = 0;

    float elapsedTime_ = 0.0f;
    float bestTime_ = -1.0f;

    bool finished_ = false;
};
