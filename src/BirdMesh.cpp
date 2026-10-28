
#include "BirdMesh.h"
#include "rlgl.h"

#include <cmath>

namespace
{
    // Name: DrawFacet
    // Description: Draws a triangle visible from both sides
    // Parameters: Three vertices and a color
    // Returns: None
    void DrawFacet(Vector3 a, Vector3 b, Vector3 c, Color color)
    {
        DrawTriangle3D(a, b, c, color);
        DrawTriangle3D(c, b, a, color);
    }

    // Name: EllipsoidPoint
    // Description: Calculates a point on an ellipsoid
    // Parameters: Center, radii, latitude, longitude
    // Returns: Vector3 position
    Vector3 EllipsoidPoint(
        Vector3 center,
        Vector3 radii,
        float latitude,
        float longitude)
    {
        return {
            center.x + radii.x * std::cos(latitude) * std::cos(longitude),
            center.y + radii.y * std::sin(latitude),
            center.z + radii.z * std::cos(latitude) * std::sin(longitude)
        };
    }

    // Name: DrawEllipsoid
    // Description: Generates a low-poly ellipsoid from triangles
    // Parameters: Center, radii, rings, segments, color
    // Returns: None
    void DrawEllipsoid(
        Vector3 center,
        Vector3 radii,
        int rings,
        int segments,
        Color color)
    {
        for (int i = 0; i < rings; i++)
        {
            float lat0 = -PI / 2.0f + PI * i / rings;
            float lat1 = -PI / 2.0f + PI * (i + 1) / rings;

            for (int j = 0; j < segments; j++)
            {
                float lon0 = 2.0f * PI * j / segments;
                float lon1 = 2.0f * PI * (j + 1) / segments;

                Vector3 a = EllipsoidPoint(center, radii, lat0, lon0);
                Vector3 b = EllipsoidPoint(center, radii, lat1, lon0);
                Vector3 c = EllipsoidPoint(center, radii, lat1, lon1);
                Vector3 d = EllipsoidPoint(center, radii, lat0, lon1);

                // Alternate colors to emphasize polygon faces
                Color facetColor = color;

                if ((i + j) % 3 == 0)
                {
                    facetColor = ColorBrightness(color, -0.12f);
                }

                DrawFacet(a, b, c, facetColor);
                DrawFacet(a, c, d, facetColor);
            }
        }
    }

    // Name: DrawWing
    // Description: Generates and rotates a triangular bird wing
    // Parameters: left side flag, wing angle
    // Returns: None
    void DrawWing(bool left, float wingAngle)
    {
        float side = left ? -1.0f : 1.0f;

        rlPushMatrix();

        // Position the wing at its shoulder
        rlTranslatef(side * 0.38f, 0.12f, -0.15f);

        // Flap both wings symmetrically
        rlRotatef(left ? -wingAngle : wingAngle, 0.0f, 0.0f, 1.0f);

        Vector3 root = { 0.0f, 0.0f, 0.0f };
        Vector3 front = { side * 0.8f, 0.0f, -0.25f };
        Vector3 tip = { side * 2.1f, 0.0f, -0.15f };
        Vector3 back = { side * 1.6f, 0.0f, 0.55f };
        Vector3 inner = { side * 0.55f, 0.0f, 0.35f };

        Color wingColor = { 139, 89, 57, 255 };
        Color featherColor = { 105, 65, 43, 255 };

        DrawFacet(root, front, tip, wingColor);
        DrawFacet(root, tip, back, featherColor);
        DrawFacet(root, back, inner, wingColor);

        rlPopMatrix();
    }

    // Name: DrawTail
    // Description: Generates the bird's triangular tail feathers
    // Parameters: None
    // Returns: None
    void DrawTail()
    {
        Color tailColor = { 110, 70, 48, 255 };

        Vector3 root = { 0.0f, 0.0f, 0.55f };
        Vector3 left = { -0.55f, 0.02f, 1.35f };
        Vector3 middle = { 0.0f, 0.02f, 1.55f };
        Vector3 right = { 0.55f, 0.02f, 1.35f };

        DrawFacet(root, left, middle, tailColor);
        DrawFacet(root, middle, right, tailColor);
    }

    // Name: DrawBeak
    // Description: Generates a small triangular beak
    // Parameters: None
    // Returns: None
    void DrawBeak()
    {
        Color beakColor = { 235, 170, 45, 255 };

        Vector3 tip = { 0.0f, 0.08f, -1.35f };
        Vector3 top = { 0.0f, 0.20f, -0.94f };
        Vector3 bottom = { 0.0f, -0.02f, -0.94f };
        Vector3 left = { -0.15f, 0.08f, -0.94f };
        Vector3 right = { 0.15f, 0.08f, -0.94f };

        DrawFacet(top, left, tip, beakColor);
        DrawFacet(top, tip, right, beakColor);
        DrawFacet(bottom, tip, left, GOLD);
        DrawFacet(bottom, right, tip, GOLD);
    }
}

// Name: Draw
// Description: Draws the complete procedural bird
// Parameters: wingAngle in degrees
// Returns: None
void BirdMesh::Draw(float wingAngle) const
{
    // Body
    DrawEllipsoid(
        { 0.0f, 0.0f, 0.0f },
        { 0.45f, 0.32f, 0.75f },
        6, 10,
        { 155, 103, 68, 255 }
    );

    // Head
    DrawEllipsoid(
        { 0.0f, 0.16f, -0.68f },
        { 0.30f, 0.30f, 0.30f },
        5, 8,
        { 170, 117, 78, 255 }
    );

    // Eyes
    DrawEllipsoid(
        { -0.24f, 0.24f, -0.82f },
        { 0.055f, 0.065f, 0.055f },
        4, 6,
        BLACK
    );

    DrawEllipsoid(
        { 0.24f, 0.24f, -0.82f },
        { 0.055f, 0.065f, 0.055f },
        4, 6,
        BLACK
    );

    // Beak
    DrawBeak();

    // Wings
    DrawWing(true, wingAngle);
    DrawWing(false, wingAngle);

    // Tail
    DrawTail();
}
