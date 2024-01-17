#pragma once

struct Vertex_Bagic
{
    Vertex_Bagic() {}
    Vertex_Bagic(float x, float y, float z)
        : pos(x, y, z)
    {

    }

    XMFLOAT3 pos;
};


struct Vertex
{
    Vertex() {}
    Vertex(float x, float y, float z)
        : pos(x, y, z)
    {

    }

    XMFLOAT3 pos;
    XMFLOAT2 uv;
};
