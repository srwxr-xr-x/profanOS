#include <syscall.h>

#define PI 3.14159265358979323846
#define MATH_LOOP 100
#define focal_distance 100

typedef struct Point3_t {
    int x;
    int y;
    int z;
} Point3_t;

typedef struct Point2_t {
    int x;
    int y;
} Point2_t;

typedef struct Line_t {
    int i1;
    int i2;
} Line_t;

typedef struct Shape_t {
    Point3_t* Points;
    Line_t* Lines;
    Point2_t* ScreenPoints;
    int PointsCount;
    int LinesCount;
} Shape_t;

Shape_t cube(int size);
void delete_shape(Shape_t* shape);

Shape_t rotate(Shape_t* shape, int x, int y, int z);
void draw(Shape_t* shape);
int show_fps(int time);


int main(int argc, char** argv) {
    c_vga_320_mode();
    c_vgui_setup(0);

    Shape_t shape = cube(120);
    int time;

    for (int i = 0; 1; i = (i + 1) % 360) {
        Shape_t new_shape = rotate(&shape, i, i, i);
        draw(&new_shape);
        delete_shape(&new_shape);
        // c_ms_sleep(10);
        time = show_fps(time);
        c_vgui_render();
    }

    c_vga_text_mode();
    return 0;
}

Point2_t project(Point3_t point) {
    int x = point.x * focal_distance / (point.z + focal_distance + 256);
    int y = point.y * focal_distance / (point.z + focal_distance + 256);
    return (Point2_t){x, y};
}

Shape_t cube(int size) {
    Shape_t shape;
    shape.PointsCount = 8;
    shape.LinesCount = 18;
    shape.Points = c_malloc(sizeof(Point3_t) * shape.PointsCount);
    shape.Lines = c_malloc(sizeof(Line_t) * shape.LinesCount);

    shape.Points[0] = (Point3_t){ size,  size,  size};
    shape.Points[1] = (Point3_t){ size, -size,  size};
    shape.Points[2] = (Point3_t){-size, -size,  size};
    shape.Points[3] = (Point3_t){-size,  size,  size};
    shape.Points[4] = (Point3_t){ size,  size, -size};
    shape.Points[5] = (Point3_t){ size, -size, -size};
    shape.Points[6] = (Point3_t){-size, -size, -size};
    shape.Points[7] = (Point3_t){-size,  size, -size};

    shape.Lines[0] = (Line_t){0, 1};
    shape.Lines[1] = (Line_t){1, 2};
    shape.Lines[2] = (Line_t){2, 3};
    shape.Lines[3] = (Line_t){3, 0};
    shape.Lines[4] = (Line_t){4, 5};
    shape.Lines[5] = (Line_t){5, 6};
    shape.Lines[6] = (Line_t){6, 7};
    shape.Lines[7] = (Line_t){7, 4};
    shape.Lines[8] = (Line_t){0, 4};
    shape.Lines[9] = (Line_t){1, 5};
    shape.Lines[10] = (Line_t){2, 6};
    shape.Lines[11] = (Line_t){3, 7};
    // make squares into triangles
    shape.Lines[12] = (Line_t){0, 2};
    shape.Lines[13] = (Line_t){4, 6};
    shape.Lines[14] = (Line_t){0, 5};
    shape.Lines[15] = (Line_t){1, 6};
    shape.Lines[16] = (Line_t){2, 7};
    shape.Lines[17] = (Line_t){3, 4};

    shape.ScreenPoints = c_malloc(sizeof(Point2_t) * shape.PointsCount);
    return shape;
}

void delete_shape(Shape_t* shape) {
    c_free(shape->Points);
    c_free(shape->Lines);
    c_free(shape->ScreenPoints);
}

double cos(int angle) {
    // convert to radians
    double x = angle * PI / 180;
    // cos of x in radians
    double res = 1;
    double pow = 1;
    double fact = 1;
    for (int i = 0; i < MATH_LOOP; i++) {
        pow *= -1 * x * x;
        fact *= (2 * i + 1) * (2 * i + 2);
        res += pow / fact;
    }
    return res;
}

double sin(int angle) {
    // convert to radians
    double x = angle * PI / 180;
    // sin of x in radians
    double res = x;
    double pow = x;
    double fact = 1;
    for (int i = 0; i < MATH_LOOP; i++) {
        pow *= -1 * x * x;
        fact *= (2 * i + 2) * (2 * i + 3);
        res += pow / fact;
    }
    return res;    
}

Shape_t rotate(Shape_t* shape, int x, int y, int z) {
    /* new object is required because the
     * floating point math is not exact 
     * and the shape will be deformed */
    Shape_t new_shape;
    new_shape.PointsCount = shape->PointsCount;
    new_shape.LinesCount = shape->LinesCount;
    new_shape.Points = c_malloc(sizeof(Point3_t) * new_shape.PointsCount);
    new_shape.Lines = c_malloc(sizeof(Line_t) * new_shape.LinesCount);
    new_shape.ScreenPoints = c_malloc(sizeof(Point2_t) * new_shape.PointsCount);
    int x1, y1, z1;
    for (int i = 0; i < new_shape.PointsCount; i++) {
        x1 = shape->Points[i].x;
        y1 = shape->Points[i].y;
        z1 = shape->Points[i].z;
        new_shape.Points[i].x = x1 * cos(z) - y1 * sin(z);
        new_shape.Points[i].y = x1 * sin(z) + y1 * cos(z);
        x1 = new_shape.Points[i].x;
        y1 = new_shape.Points[i].y;
        new_shape.Points[i].x = x1 * cos(y) + z1 * sin(y);
        new_shape.Points[i].z = -x1 * sin(y) + z1 * cos(y);
        x1 = new_shape.Points[i].x;
        z1 = new_shape.Points[i].z;
        new_shape.Points[i].y = y1 * cos(x) - z1 * sin(x);
        new_shape.Points[i].z = y1 * sin(x) + z1 * cos(x);
    }
    for (int i = 0; i < new_shape.LinesCount; i++) {
        new_shape.Lines[i] = shape->Lines[i];
    }
    return new_shape;
}

void draw(Shape_t* shape) {
    c_vgui_clear(0);
    for (int i=0; i<shape->PointsCount; i++) {
        Point2_t p = project(shape->Points[i]);
        shape->ScreenPoints[i] = p;
    }
    for (int i=0; i<shape->LinesCount; i++) {
        Line_t line = shape->Lines[i];
        Point2_t p1 = shape->ScreenPoints[line.i1];
        Point2_t p2 = shape->ScreenPoints[line.i2];
        c_vgui_draw_line(p1.x+100, p1.y+100, p2.x+100, p2.y+100, 63);
    }
}

int show_fps(int time) {
    int new_time = c_timer_get_tick();
    int fps = 1000 / (new_time - time);
    char fps_str[10];
    c_int_to_ascii(fps, fps_str);
    c_vgui_print(0, 0, fps_str, 1, 15);
    return new_time;
}
