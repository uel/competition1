#include "stdio.h"
#include "limits.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

int ReadStart(int* width)
{
    if (getchar() !='+')
        return 1;

    char ch = getchar();
    do
    {
        if (ch != '-' || getchar() != '-' || getchar() != '+')
            return 1;

        (*width)++;
        ch = getchar();
    } while (ch != '\n' && *width <= 32);

    if ((*width) > 32)
       return 1;
    
    return 0;
}

int ReadNumber()
{
    int res = 0;
    int ch = getchar();
    if (ch == ' ');
    else if (ch <= '9' && ch > '0')
        res += 10*(ch-'0');
    else
        return -1;
    
    ch = getchar();
    if (ch == ' ')
    {
        if (res != 0)
            res/=10;
    }
    else if (ch <= '9' && ch >= '0')
    {
        if (res == 0 && ch == '0')
            return -1;
        res += ch-'0';
    }
    else
        return -1;
    
    return res;
}

int ReadRow(int*** field, int* width, int* height, int* numCount)
{
    if (getchar() != '|')
        return 1;
        
    for (int j = 0; j < *width; j++)
    {
        int n = ReadNumber();
        if (n == -1)
            return 1;
        
        (*field)[*height][j] = n;
        if (n != 0)
            (*numCount)++;

        char ch = getchar();
        if ((j != (*width)-1 && ch != ' ') || (j == (*width)-1 && ch != '|'))
            return 1;
    }
    if (getchar() != '\n')
        return 1;

    return 0;
}

int ReadSpace(int* width)
{
    if (getchar() !='+')
        return -1;

    char ch = getchar();

    if (ch == '-')
        return 1;
    else if (ch != ' ' || getchar() != ' ' || getchar() != '+')
        return -1;
    
    for (int i = 1; i < *width; i++)
    {
        if (getchar() != ' ' || getchar() != ' ' || getchar() != '+')
            return -1; 
    }
    if (getchar() != '\n')
        return -1;
    
    return 0;
}

int ReadEnd(int* width)
{
    if (getchar() != '-' || getchar() != '+')
        return -1;

    for (int i = 1; i < *width; i++) {
        if (getchar() != '-' || getchar() != '-' || getchar() != '+')
            return -1;
    }
    if (getchar() != '\n' || getchar() != -1)
         return -1;
    
    return 0;
}

int ReadField(int*** field, int* width, int* height)
{
    *field = (int**)calloc(32, sizeof(int*));
    for (int i = 0; i < 32; i++)
        (*field)[i] = (int*)calloc(32, sizeof(int));

    if(ReadStart(width))
        return 1;

    int numCount = 0;
    int s = 0;
    do
    {
        if (ReadRow(field, width, height, &numCount) || numCount > 200)
            return 1;
        s = ReadSpace(width);
        (*height)++;
    } while (!s && (*height) < 32);
    if (s == -1 || *height == 32)
        return 1;

    if (ReadEnd(width))
        return 1;
    
    return 0;
}

struct PERMUTATION;

typedef struct RECTANGLE
{
    int x, y, value;
    int pCount, pSize;
    int placed;
    int pMax;
    struct PERMUTATION* p;
} Rectangle;

typedef struct PERMUTATION
{
    Rectangle* rectangle;
    int x, y; //Upper leftmost corner of rectangle
    int w, h;
    int valid;
    struct PERMUTATION** links;
    int linkCount;
} Permutation;

typedef struct 
{
    int n;
    int pCount;
    int pMax;
    Rectangle* placed;
    int recAnchor;
    Permutation** p;
} PNode;

typedef struct 
{
    int w;
    int h;
    PNode** f;
} Field;

int emptyCount;

int GetRectangles(Rectangle** res, Field field, int** nfield)
{
    *res = (Rectangle*)calloc(200, sizeof(Rectangle));
    int count = 0;
    for (int i = 0; i < field.h; i++)
    {
        for (int j = 0; j < field.w; j++)
        {
            if (nfield[i][j])
            {
                (*res)[count].x = j;
                (*res)[count].y = i;
                (*res)[count].value = nfield[i][j];
                (*res)[count].pCount = 0;
                (*res)[count].pSize = 1;
                (*res)[count].p = (Permutation*)calloc(200, sizeof(Permutation));
                count++;

                field.f[i][j].n = nfield[i][j];
                field.f[i][j].recAnchor = 1;
            }
        }
    }
    *res = (Rectangle*)realloc(*res, sizeof(Rectangle)*count);
    return count;
}

int Factors(int* result, int number)
{
    int factorCount = 0;
    for (int n = 1; n <= number; n++)
    {
        if (number%n == 0)
        {
            result[factorCount] = n;
            factorCount++;
        }
    }
    return factorCount;
}

int IsInPermutation(int x, int y, Permutation* p)
{
    if (p == NULL)
        return 2;
    
    if ((p->x) <= x && x < (p->x) + (p->w) && (p->y) <= y && y < (p->y) + (p->h) )
        return 1;

    return 4;
}

int IsPlaceable(Permutation p, Field field)
{
    int nCount = 0;
    for (int i = p.y; i < p.y+p.h; i++)
    {
        for (int j = p.x; j < p.x+p.w; j++)
        {
            if (i < 0 || i > field.h-1 || j < 0 || j > field.w-1)
                return 0;
            if (field.f[i][j].n != 0)
            {
                if (field.f[i][j].n == p.rectangle->value)
                    nCount++;
                else
                    return 0;
            } 
            if (field.f[i][j].placed || nCount > 1)
                return 0;
        }
    }
    return 1;
}

Permutation* NValidPerm(Rectangle rectangle, int n)
{
    for (int i = 0; i < rectangle.pMax; i++)
    {
        if (rectangle.p[i].valid)
        {
            n--;
            if (n)
                continue;
            
            return (Permutation*)rectangle.p + i;
        }
    }
    return NULL;
}

Permutation* ValidPermFromNode(PNode node)
{
    for (int i = 0; i < node.pMax; i++)
    {
        if (node.p[i]->valid)
        {
            return (Permutation*)node.p[i];
        }
    }
    return NULL;
}

void AddPermutationToField(Permutation* p, Field field)
{
    for (int i = p->y; i < p->y+p->h; i++)
    {
        for (int j = p->x; j < p->x+p->w; j++)
        {
            field.f[i][j].p[field.f[i][j].pCount] = p;
            (field.f[i][j].pCount)++;
            (field.f[i][j].pMax)++;    
        }
    }
}

void AddPermutation(Rectangle* rectangle, Permutation p, Field field)
{
    rectangle->p[rectangle->pCount] = p;
    (rectangle->pCount)++;
    (rectangle->pMax)++;

    AddPermutationToField(rectangle->p + rectangle->pCount - 1, field);
}

void AddLink(Permutation* cause, Permutation* effect)
{
    if (cause->linkCount == 0)
        cause->links = (Permutation**)calloc(16, sizeof(Permutation*));
    if (cause->linkCount >= 16 && !(cause->linkCount & (cause->linkCount - 1)))
        cause->links = (Permutation**)realloc(cause->links, sizeof(Permutation*)*cause->linkCount*2);
    
    cause->links[cause->linkCount] = effect;
    (cause->linkCount)++;
}

int RemovePermutationFromField(Permutation* p, Field field)
{
    int isZero = 0;
    for (int i = p->y; i < p->y+p->h; i++)
    {
        for (int j = p->x; j < p->x+p->w; j++)
        {
            field.f[i][j].pCount--;
            if (!field.f[i][j].placed)
            {
                if (field.f[i][j].pCount == 0)
                    isZero = 1;
            }
        }
    }
    return isZero;
}

//return 1 if invalid field / rectangle pCount
int RemovePermutation(Permutation* p, Field field)
{
    (p->rectangle->pCount)--;
    p->valid = 0;
    if (RemovePermutationFromField(p, field) || (!p->rectangle->placed && p->rectangle->pCount == 0 ))
        return 1;

    return 0;
}

void ReAddPermutationToField(Permutation* p, Field field)
{
    for (int i = p->y; i < p->y+p->h; i++)
    {
        for (int j = p->x; j < p->x+p->w; j++)
        {
            (field.f[i][j].pCount)++;
        }
    }
}

void ReAddPermutation(Permutation* p, Field field)
{
    (p->rectangle->pCount)++;
    p->valid = 1;
    ReAddPermutationToField(p, field);
}

Permutation* placementCache[2000];
int cacheCount;

void Track(Permutation* p)
{
    placementCache[cacheCount] = p;
    cacheCount++;
}

void Undo(Permutation* p, int nLastPlacements, Field field)
{
    p->rectangle->placed = 0;
    emptyCount += p->w * p->h;
    for (int i = p->y; i < p->y+p->h; i++)
    {
        for (int j = p->x; j < p->x+p->w; j++)
        {
            if (!field.f[i][j].recAnchor)
                field.f[i][j].n = 0;
            
            field.f[i][j].placed = NULL;
        }
    }

    for (int i = cacheCount-1; i >= cacheCount-nLastPlacements; i--)
    {
        ReAddPermutation(placementCache[i], field);
    }
    cacheCount -= nLastPlacements;
}

int placeCount;
//mode == 0 no undo, mode == 1 undo, mode == 2 undo + no cascading links
int Place(Permutation* p, Field field, int mode)
{
    p->rectangle->placed = 1;
    emptyCount -= p->w * p->h;
    int nRemoved = 0;
    for (int i = p->y; i < p->y+p->h; i++)
    {
        for (int j = p->x; j < p->x+p->w; j++)
        {
            field.f[i][j].n = p->rectangle->value;
            field.f[i][j].placed = p->rectangle;
        }
    }

    for (int i = p->y; i < p->y+p->h; i++)
    {
        for (int j = p->x; j < p->x+p->w; j++)
        {
            for (int k = 0; k < field.f[i][j].pMax; k++)
            {
                if (field.f[i][j].p[k]->valid)
                {
                    if (mode) 
                    {
                        Track(field.f[i][j].p[k]);
                        nRemoved++;
                        if(RemovePermutation(field.f[i][j].p[k], field))
                            return nRemoved*-1;
                    }
                    else
                        RemovePermutation(field.f[i][j].p[k], field);
                }
            }
        }
    }
  
    if (!(nRemoved < 0) && mode == 1)
    {
        for (int i = 0; i < p->linkCount; i++)
        {
            if (!(p->rectangle->placed) && p->links[i]->valid)
            {
                int n2Removed = Place(p->links[i], field, 1);
                if (n2Removed < 0)
                {
                    nRemoved += -1*n2Removed;
                    nRemoved *= -1;
                }
                else
                {
                    nRemoved += n2Removed;
                }
            }
        }
    }
    return nRemoved;
}

void AddValidPermutations(Rectangle* rectangle, int w, int h, Field field)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            Permutation p;
            p.rectangle = rectangle;
            p.w = w;
            p.h = h;
            p.x = (rectangle->x) - j;
            p.y = (rectangle->y)- i;
            p.valid = 1;
            p.links = NULL;
            p.linkCount = 0;
            if (IsPlaceable(p, field))
            {
                AddPermutation(rectangle, p, field);
            }
        }
    }
    
}

int FindPermutations(Rectangle* rectangles, int rectangleCount, Field field)
{
    for (int i = 0; i < rectangleCount; i++)
    {
        int factors[12] = {0};
        int fc = Factors(factors, rectangles[i].value);
        for (int f = 0; f < fc; f++)
        {
            AddValidPermutations(rectangles+i, factors[f], (rectangles[i].value)/factors[f], field);
        }
        

        if (rectangles[i].pCount == 0)
            return 0;    
    }
    return 1;
}

int isBorder(Field field, int x1, int y1, int x2, int y2)
{
    if (field.f[y1][x1].placed != field.f[y2][x2].placed)
    {
        return 1;
    }
    return 0;
}

Rectangle* FindRectangle(Rectangle* rectangles, int recCount, int x, int y)
{
    for (int i = 0; i < recCount; i++)
    {
        if (rectangles[i].x == x && rectangles[i].y == y)
            return rectangles+i;
    }
    return NULL;
}

void PrintField(Rectangle* rectangles, int recCount, Field f)
{
    for (int i = 0; i < f.w; i++)
        printf("+--");
    printf("+\n");
    
    for (int i = 0; i < f.h; i++)
    {
        printf("|");
        for (int j = 0; j < f.w; j++)
        {
            Rectangle* rec = FindRectangle(rectangles, recCount, j, i);
            if (rec != NULL)
            {
                if (!((f.f[i][j].n)/10))
                    printf(" %d", f.f[i][j].n);
                else
                    printf("%d", f.f[i][j].n);
            }
            else{
                printf("  ");
            }
            
            if (j != f.w-1)
                printf(isBorder(f, j, i, j+1, i) ? "|" : " ");            
        }
        printf("|\n");

        if (i != f.h-1)
        {
            for (int j = 0; j < f.w; j++)
                printf(isBorder(f, j, i, j, i+1) ? "+--" : "+  ");            
            printf("+\n");
        }
        
    }
    
    for (int i = 0; i < f.w; i++)
        printf("+--");
    printf("+\n");
}

int Fill1Rectangles(Rectangle* rectangles, int rectangleCount, Field field)
{
    int changes = 0;
    for (int i = 0; i < rectangleCount; i++)
    {
        if (!rectangles[i].placed && rectangles[i].pCount == 1)
        {            
            Permutation* v = NValidPerm(rectangles[i], 1);
            Place(v, field, 0);
            changes++;
        }
    }
    
    return changes;
}

int Fill1Field(Field field)
{
    int changes = 0;

    for (int i = 0; i < field.h; i++)
    {
        for (int j = 0; j < field.w; j++)
        {
            if (!field.f[i][j].placed)
            {
                if (field.f[i][j].pCount == 0)
                {
                    return -1;
                }
                if (field.f[i][j].pCount == 1)
                {
                    Permutation* v = ValidPermFromNode(field.f[i][j]);
                    Place(v, field, 0);
                    changes++;
                }
            }
        }
        
    }
    
    return changes;
}

int FieldIsValid(Rectangle* rectangles, int rectangleCount, Field field)
{
    for (int i = 0; i < field.h; i++)
    {
        for (int j = 0; j < field.w; j++)
        {
            if (!field.f[i][j].placed && field.f[i][j].pCount == 0)
            {
                return 0;
            }
        }
    }

    for (int i = 0; i < rectangleCount; i++)
    {
        if (!rectangles[i].placed && rectangles[i].pCount == 0)
        {
            return 0;
        }
    }

    return 1;
}

Rectangle* GetNextUnplacedRec(Rectangle* rectangles, int rectangleCount, Rectangle* last)
{
    for (int i = (last == NULL ? 0 : last-rectangles+1); i < rectangleCount; i++)
    {
        if (!rectangles[i].placed)
        {   
            return rectangles + i;
        }
    }
    
    return NULL;
}

void BF(Rectangle* rectangles, int rectangleCount, Field field, Rectangle* prev, int* c, int saveSolution)
{
    placeCount++;
    Rectangle* r = GetNextUnplacedRec(rectangles, rectangleCount, prev);
    for (int p = 0; p < r->pMax; p++)
    {
        if (r->p[p].valid && IsPlaceable(r->p[p], field))
        {
            int nRemoved = Place(r->p + p, field, 1);
            
            if (!emptyCount)
                (*c)+=1;
            else if (nRemoved >= 0 && emptyCount > 0)
                BF(rectangles, rectangleCount, field, r, c, saveSolution);    
            if ((*c) && saveSolution)
                return;
            
            if (nRemoved < 0)
                Undo(r->p + p, nRemoved*-1, field);
            else
                Undo(r->p + p, nRemoved, field);
        }
    }
}

int BruteForce(Rectangle* rectangles, int rectangleCount, Field field, int saveSolution)
{
    int count = 0;
    BF(rectangles, rectangleCount, field, NULL, &count, saveSolution);
    return count;
}

PNode** CreatePfield(int** field, int w, int h)
{
    PNode** pfield = (PNode**)calloc(sizeof(PNode*), h);
    for (int i = 0; i < h; i++)
    {
        pfield[i] = (PNode*)calloc(sizeof(PNode), w);
        for (int j = 0; j < w; j++)
        {
            pfield[i][j].p = (Permutation**)calloc(sizeof(Permutation*), 200);
        }
    }
    return pfield;
}

Field* CreateField(int** nfield, int w, int h)
{
    Field* field = (Field*)calloc(1, sizeof(Field));
    field->f = CreatePfield(nfield, w, h);
    field->w = w;
    field->h = h;
    emptyCount = w*h;
    return field;
}

int Fill(Rectangle* rectangles, int rectangleCount, Field field)
{
    int s;
    do
    {
        s = 0;
        s = Fill1Rectangles(rectangles, rectangleCount, field);
        if(s < 0)
            return 0;

        s += Fill1Field(field);
        if(s < 0)
            return 0;
    } while (s);

    return 1;
}

void FreeRectangles(int rectangleCount, Rectangle* rectangles)
{
    for (int i = 0; i < rectangleCount; i++) {
        for (int j = 0; j < 200; j++)
            if (rectangles[i].p[j].links != NULL)
                free(rectangles[i].p[j].links);
        free(rectangles[i].p);
    }
    free(rectangles);
}

void FreeField(Field* field)
{
    for (int i = 0; i < field->h; i++) {
        for (int j = 0; j < field->w; j++)
            free(field->f[i][j].p);
        free(field->f[i]);
    }
    free(field->f);
    free(field);
}

void Link1P(Permutation* p, Rectangle* rectangles, int rectangleCount, Field field)
{
    for (int i = 0; i < field.h; i++)
    {
        for (int j = 0; j < field.w; j++)
        {
            if (!field.f[i][j].placed && field.f[i][j].pCount == 1)
            {
                AddLink(p, ValidPermFromNode(field.f[i][j]));
            }
        }
    }

    for (int i = 0; i < rectangleCount; i++)
    {
        if (!rectangles[i].placed && rectangles[i].pCount == 0)
        {
            AddLink(p, NValidPerm(rectangles[i], 1));
        }
    }
}

void FindLinks(Rectangle* rectangles, int rectangleCount, Field field, int linkingBound)
{
    for (int i = 0; i < rectangleCount; i++)
    {
        for (int j = 0; j < rectangles[i].pMax; j++)
        {
            if (rectangles[i].p[j].valid)
            {
                int nRemoved = Place(rectangles[i].p + j, field, 2);
                if (nRemoved < 0)
                {
                    Undo(rectangles[i].p + j, nRemoved*-1, field);
                    RemovePermutation(rectangles[i].p + j, field);
                }
                else
                {
                    if (rectangleCount > linkingBound)
                        Link1P(rectangles[i].p + j, rectangles, rectangleCount, field);
                    Undo(rectangles[i].p + j, nRemoved, field);
                }
            }
        }
    }
    if (rectangleCount > linkingBound)
        for (int i = 0; i < rectangleCount; i++)
        {
            for (int j = 0; j < rectangles[i].pMax; j++)
            {
                if (rectangles[i].p[j].valid)
                {
                    int nRemoved = Place(rectangles[i].p + j, field, 1);
                    if (nRemoved < 0)
                    {
                        Undo(rectangles[i].p + j, nRemoved*-1, field);
                        RemovePermutation(rectangles[i].p + j , field);
                    }
                    else
                    {
                        Undo(rectangles[i].p + j, nRemoved, field);
                    }
                }
            }
        }
}

int RectangleSum(Rectangle* rectangles, int rectangleCount)
{
    int s = 0;
    
    for (int i = 0; i < rectangleCount; i++)
        s+=rectangles[i].value;
    
    return s;
}

int Solve(Rectangle* rectangles, int rectangleCount, Field field)
{
    int count;

    if (field.w*field.h != RectangleSum(rectangles, rectangleCount))
        return -1;
    
    if(!FindPermutations(rectangles, rectangleCount, field))
        return -1;

    if(!Fill(rectangles, rectangleCount, field))
        return -1;
    else if (!emptyCount)
        return 1;
    
    FindLinks(rectangles, rectangleCount, field, 20);

    if(!Fill(rectangles, rectangleCount, field))
        return -1;
    else if (!emptyCount)
        return 1;

    count = BruteForce(rectangles, rectangleCount, field, 0);
    if(!count)
        return -1;

    if (count == 1)
        BruteForce(rectangles, rectangleCount, field, 1);

    return count;
}

int main(int argc, char const *argv[])
{
    printf("Zadejte puzzle:\n");
    int** fieldtmp;
    int w = 0, h = 0;
    if (ReadField(&fieldtmp, &w, &h))
    {
        for (int i = 0; i < 32; i++)
            free(fieldtmp[i]);
        free(fieldtmp);
        printf("Nespravny vstup.\n");
        return 1;
    }

    Field* field = CreateField(fieldtmp, w, h);

    Rectangle* rectangles = NULL;
    int rectangleCount = GetRectangles(&rectangles, *field, fieldtmp); 

    for (int i = 0; i < 32; i++)
        free(fieldtmp[i]);
    free(fieldtmp);
    
    int solutions = Solve(rectangles, rectangleCount, *field);
    
    if (solutions == 1)
    {
        printf("Jedno reseni:\n");
        PrintField(rectangles, rectangleCount, *field);
    }
    else if (solutions > 1)
    {
        printf("Celkem reseni: %d\n", solutions);
    }
    else
    {
        printf("Reseni neexistuje.\n");
    }
    
    FreeRectangles(rectangleCount, rectangles);
    FreeField(field);
    return 0;
}