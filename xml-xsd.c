#include <stdio.h>
#include <math.h>
#include <string.h>
#include <libxml/xmlschemastypes.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

struct student
{
    int studentId;
    char name[25];
    char surname[25];
    char coursecode[20];
    int credit;
    int mid1;
    int mid2;
    int hw1;
    int hw2;
    int hw3;
    int final;
    char grade[25];
    char status[25];
    
};

struct parse_gradeStudent
{
    int studentId;
    int credit;
    char coursecode[20];
    int gradeAverage;
    int graded;
};

#define BUFFER_SIZE 150
void xml_xsd_validation();
struct student saveToStruct (char* str);
static void find_allStatistics(xmlNode * a_node);
static void completeStd(xmlNode * a_node,int avrg,int count, char name[15]);
void printResult(char temp[15],int max,int min,int average,int count);

int main()
{
    struct student arr[34];
    FILE* file = fopen("input.csv", "r");
    if (!file)
        return -1;

    char buffer[BUFFER_SIZE];
    int flag = 0,permit=0;
    while (fgets(buffer, BUFFER_SIZE, file))
    {
        if(0!=permit){
        arr[flag] = saveToStruct(buffer); //Split etme işlemi metot olarak yazıldı(saveToStruct).
        flag++;
        }
        if(flag==34){
        permit=0;//input.csv'nin en sonundaki bosluğu okuması engellendi.
        }
        else
        permit=1; 
        
    }
   ///////////////////////////////////////////////////////////////
   //Dosyadan(input.csv) girdiler okunarak Struct array'e(arr[34]) yerleştirildi.
   ///////////////////////////////////////////////////////////////

   xmlDocPtr doc = NULL; /* document pointer */
   xmlNodePtr root_node = NULL, node = NULL, node1 = NULL, node2 = NULL; /* node pointers */
   char convert[256]; /* used to convert int to string*/
   
   int i,j;
   doc = xmlNewDoc (BAD_CAST "1.0");
   root_node = xmlNewNode (NULL, BAD_CAST "Lectures");
   xmlDocSetRootElement( doc, root_node);
   
   
   char term[5][10];
   permit=1;
   flag=1;
   for (i = 0; i < 34; i++){
       if(i==0)
       {
           sprintf(term[0], arr[0].coursecode);
       }
       else{
           for (j = 0; j < permit; j++){
               if(strcmp(term[j], arr[i].coursecode)==0){
                  flag=0; 
                }
            }
            if(flag==1){
                sprintf(term[permit], arr[i].coursecode);
                permit++;
            }
            flag=1;

        }
       
   }
   /*
   *Root_node taslağı için dökümantasyondan CourseCode'ler çekildi.
   *Root ağaç mantığı='Her dersin öğrencileri vardır' olarak oluşturuldu.
   */
  
  
   for (i = 0; i < 5; i++) {
    
      if(0==i){
          node = xmlNewChild(root_node, NULL, BAD_CAST "Course", NULL);
          xmlNewProp(node, BAD_CAST "Code", BAD_CAST term[0]);
      }
      else if(1==i){
          node = xmlNewChild(root_node, NULL, BAD_CAST "Course", NULL);
          xmlNewProp(node, BAD_CAST "Code", BAD_CAST term[1]);
      }
      else if(2==i){
          node = xmlNewChild(root_node, NULL, BAD_CAST "Course", NULL);
          xmlNewProp(node, BAD_CAST "Code", BAD_CAST term[2]);
      }
      else if(3==i){
          node = xmlNewChild(root_node, NULL, BAD_CAST "Course", NULL);
          xmlNewProp(node, BAD_CAST "Code", BAD_CAST term[3]);
      }
      else{
          node = xmlNewChild(root_node, NULL, BAD_CAST "Course", NULL);
          xmlNewProp(node, BAD_CAST "Code", BAD_CAST term[4]);
      }

   /*
   * Root_node taslağı oluşturuldu.
   */


       for (j = 0; j < 34; j++) {

           if(strcmp(term[i], arr[j].coursecode)==0){
             node1 = xmlNewChild(node, NULL, BAD_CAST "Student", NULL);

             sprintf(convert, "%d", arr[j].studentId);
             node2 = xmlNewChild(node1, NULL, BAD_CAST "StudentId", convert);
             
             node2 = xmlNewChild(node1, NULL, BAD_CAST "Name", arr[j].name);
             node2 = xmlNewChild(node1, NULL, BAD_CAST "Surname", arr[j].surname);
             node2 = xmlNewChild(node1, NULL, BAD_CAST "Course-Code", arr[j].coursecode);

             sprintf(convert, "%d", arr[j].credit);
             node2 = xmlNewChild(node1, NULL, BAD_CAST "Course-Credit", convert);
             sprintf(convert, "%d", arr[j].mid1);
             node2 = xmlNewChild(node1, NULL, BAD_CAST "MID1", convert);
             sprintf(convert, "%d", arr[j].mid2);
             node2 = xmlNewChild(node1, NULL, BAD_CAST "MID2", convert);
             sprintf(convert, "%d", arr[j].hw1);
             node2 = xmlNewChild(node1, NULL, BAD_CAST "HW1", convert);
             sprintf(convert, "%d", arr[j].hw2);
             node2 = xmlNewChild(node1, NULL, BAD_CAST "HW2", convert);
             sprintf(convert, "%d", arr[j].hw3);
             node2 = xmlNewChild(node1, NULL, BAD_CAST "HW3", convert);
             sprintf(convert, "%d", arr[j].final);

             node2 = xmlNewChild(node1, NULL, BAD_CAST "Final", convert);
             node2 = xmlNewChild(node1, NULL, BAD_CAST "Grade", arr[j].grade);
             node2 = xmlNewChild(node1, NULL, BAD_CAST "Status", arr[j].status);

             }

         }
         /*
         * Tüm Dökümanlar Kullanılarak XML dolduruldu.
         */
       
   }
   
   
   xmlSaveFormatFileEnc ("xml-xsd.xml", doc, "UTF-8", 1);
   xmlFreeDoc(doc);
   xmlCleanupParser();
   xmlMemoryDump ();
   ///////////////////////////////////////////////////////////////
   //arr[34] kullanılarak XML oluşturuldu.
   ///////////////////////////////////////////////////////////////

   xml_xsd_validation();

   ///////////////////////////////////////////////////////////////
   //XML/XSD validation yapıldı.
   ///////////////////////////////////////////////////////////////

   xmlDoc *parsedoc = NULL;
   xmlNode *root_element = NULL;
   const char *Filename = "xml-xsd.xml";
   parsedoc = xmlReadFile (Filename, NULL, 0);
   if (parsedoc == NULL)
    printf("error: could not parse file %s\n", Filename);
   else
   { 
    root_element = xmlDocGetRootElement (parsedoc);
    find_allStatistics(root_element);
    //Tüm istatistik işlemleri 'find_allStatistics' metodu içinde yapıldı.
    //Toplamda XML 2 kere dolanıldı.
     xmlFreeDoc (parsedoc);
   }
   xmlCleanupParser ();

   ////////////////////////////////////////////////////////////////////
   //İstatistikler, find_allStatistics metotu kullanılarak hesaplandı.
   ////////////////////////////////////////////////////////////////////



    return 0;
}


static void find_allStatistics(xmlNode * a_node)
{
   static struct parse_gradeStudent gradeStudent[34];
   static int max=INT_MIN,min=INT_MAX;
   static int average=0,count=0,totalcount=0, totalgrade,flag=0;
   static char temp[15]="temp";
   xmlNode *cur_node = NULL;
   
   for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
       if (cur_node->type == XML_ELEMENT_NODE ) {
         
         /////////////
         if(strcmp("Course-Code", cur_node->name) == 0 && strcmp(temp, cur_node->children->content) != 0){
             if(strcmp(temp, "temp") != 0){
                printResult(temp,max,min,average,count);

                max=INT_MIN;
                min=INT_MAX;
                average=0;
                count=0;
             }
             strcpy(temp,cur_node->children->content);
         }
         
         if(strcmp("Final", cur_node->name) == 0){
             if(atoi(cur_node->children->content)>max)
                max=atoi(cur_node->children->content);
             if(atoi(cur_node->children->content)<min)
                min=atoi(cur_node->children->content);
            
            average+=atoi(cur_node->children->content);
            count++;
            if(totalcount==33){
                printResult(temp,max,min,average,count);
                max=INT_MIN;
                min=INT_MAX;
                average=0;
                count=0;
            }
         }
         //////////Max-Min-Average-Std ağaç dolaşılarak hesaplandı.

         if(strcmp("StudentId", cur_node->name) == 0){
             gradeStudent[totalcount].studentId=atoi(cur_node->children->content);
             gradeStudent[totalcount].graded=0;
             totalgrade=0;
         }
         else if(strcmp("Course-Code", cur_node->name) == 0){
             strcpy(gradeStudent[totalcount].coursecode, cur_node->children->content);
             if(strcmp(gradeStudent[totalcount].coursecode,"CME3004")==0)
                  flag=1;
             else if(strcmp(gradeStudent[totalcount].coursecode,"CME3006")==0)
                  flag=2;
         }
         else if(strcmp("Course-Credit", cur_node->name) == 0)
             gradeStudent[totalcount].credit=atoi(cur_node->children->content);

         else if(strcmp("MID1", cur_node->name) == 0 || strcmp("MID2", cur_node->name) == 0 || strcmp("HW1", cur_node->name) == 0 || strcmp("HW2", cur_node->name) == 0 || strcmp("HW3", cur_node->name) == 0){
             totalgrade+= ((atoi(cur_node->children->content))*15)/100;
             }
         else if(strcmp("Final", cur_node->name) == 0){
             if(flag==0)
                 totalgrade+=((atoi(cur_node->children->content))*25)/100;
            else if(flag==1)
                 totalgrade+=((atoi(cur_node->children->content))*40)/100;
            else
                 totalgrade+=((atoi(cur_node->children->content))*55)/100;
             flag=0;
             gradeStudent[totalcount].gradeAverage=totalgrade;
             totalcount++;
         }
         //Mid1-Mid2-HW1-HW2-Hw3--%15 Final-%25 Alındı.
         //CME3004 için Mid1-Mid2-HW1-HW2--%15 Final-%40 Alındı.
         //CME3006 için Mid1-HW1-HW2--%15 Final-%55 Alındı.
         ///////////////////////////////////////////////////////

         //tüm öğrencilerin eniyi/enkötü dersleri bulundu.code
         //her öğrencinin not ortalaması bulundu.(herdersin kredileride kullanıldı.)
         if(totalcount==34){
             totalcount=0;//tekrar girmesi engellendi.
             char crscodeMin[15],crscodeMax[15];
             int i=0,j=0,k=0;
             int GPA=0,counter=0;
             for (i = 0; i < 34; i++){
                 min=100;
                 max=0;
            if(gradeStudent[i].graded==0){
               for (j = i; j < 34; j++){
                   if(gradeStudent[i].studentId==gradeStudent[j].studentId){
                       if(gradeStudent[j].gradeAverage < min){
                           min=gradeStudent[j].gradeAverage;
                           strcpy(crscodeMin,gradeStudent[j].coursecode);
                        }
                        if(gradeStudent[j].gradeAverage > max){
                           max=gradeStudent[j].gradeAverage;
                           strcpy(crscodeMax,gradeStudent[j].coursecode);
                        }
                        GPA+=gradeStudent[j].credit*gradeStudent[j].gradeAverage;
                        counter+=gradeStudent[j].credit;
                        //Öğrencinin dersteki ortalama notu(Mid1%15+Mid2%15...) kredi ile çarpılarak eklendi.coursecode
                        //En son işlemde total sayı bu kredilere bölünerek GPA bulundu.
                        }
               }
               for (k = i+1; k < 34; k++)
                   if(gradeStudent[k].studentId==gradeStudent[i].studentId)
                      gradeStudent[k].graded=1;
            }
                   
               if(gradeStudent[i].graded==0){
                   GPA=GPA/counter;
                  printf("For the student->%d\n",gradeStudent[i].studentId);
                  printf("The best graded course : %s\n",crscodeMax);
                  printf("The worst graded course: %s\n",crscodeMin);
                  printf("GradePointAverage(GPA) : %d\n",GPA);
                  printf("-------------------------------------------------------\n");
                  gradeStudent[i].graded=1;
                  GPA=0;
                  counter=0;
               }
            }
         }

         


       }
        
        find_allStatistics(cur_node->children);
   }
}

static void completeStd(xmlNode * a_node,int avrg,int count, char name[15])
{
   //Metoda ortalama ve dersin kodu gönderilerek standart sapma hesabı yapıldı.
   static int total=0,flag=0,number=0;
   xmlNode *cur_node = NULL;
   for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
       if (cur_node->type == XML_ELEMENT_NODE ) {
         
         if(strcmp("Course-Code", cur_node->name) == 0 && strcmp(name, cur_node->children->content) == 0)
             flag=1;
          

         if(flag==1 && strcmp("Final", cur_node->name) == 0){
            int temp=abs(atoi(cur_node->children->content)-avrg);
            total+=temp*temp;
            flag=0;
            number++;

            if(number==count){
              total=(total/(count-1));
              total=sqrt(total);
              printf("Std.deviation for %s: %d\n",name,total);
              total=0;
              number=0;
            }
         }

             
       }
        
        completeStd(cur_node->children,avrg,count,name);
   }
}


struct student saveToStruct (char* str)
{
    struct student res;
    int flag = 0,permit=0;
    char *token = strtok(str, ","); 

    while( token != NULL )
    {
        if (0 == flag)
            res.studentId = atoi(token);
        else if (1 == flag)
            strcpy(res.name, token);
        else if (2 == flag)
            strcpy(res.surname, token);
        else if (3 == flag){
            strcpy(res.coursecode, token);
            if(strcmp(token,"CME3006")==0)
                permit=1;
            else if(strcmp(token,"CME3004")==0)
                permit=2;
            //Girdisi olmayan dersler/bölümler için istisna olusturuldu.
        }
        else if (4 == flag)
             res.credit= atoi(token);
        else if (5 == flag)
             res.mid1= atoi(token);
        else if (6 == flag)
             if(permit==1){
                res.mid2=0;
                flag=7;
                //#1
             }
             else
                 res.mid2= atoi(token);
        if (7 == flag)
             res.hw1= atoi(token);
        else if (8 == flag)
             res.hw2= atoi(token);
        else if (9 == flag){
            if(permit==0)
             res.hw3= atoi(token);
            else{
             res.hw3= 0;
             permit=0;
             flag=10;
             //#2
             //#1 ve #2 de cme3004 ve cme3006 için iki virgül olayı çözüldü.code
             //ilk olarak '0' atandı fakat ortalamada etkisi alınmadı.(@find_allStatistics metodunda)
            }
        }
        if (10 == flag)
             res.final= atoi(token);
        else if (11 == flag)
             strcpy(res.grade, token);
        else if (12 == flag){
             strcpy(res.status, token);
             res.status[ strlen(res.status) - 2 ] = '\0';
             //XML yazdırılırken sonda çıkan '&#13' komutu silindi.
        }
        
        flag++;
        token = strtok( NULL, "," ); 
    }
    return res;
}

void printResult(char temp[15],int max,int min,int average,int count){
    printf("Maximum grade for %s: %d\n",temp,max);
    printf("Minimum grade for %s: %d\n",temp,min);
    printf("Average grade for %s: %d\n",temp,average/count);

    xmlDoc *parsedoc = NULL;
    xmlNode *root_element = NULL;
    const char *Filename = "xml-xsd.xml";
    parsedoc = xmlReadFile (Filename, NULL, 0);
    root_element = xmlDocGetRootElement (parsedoc);
    completeStd(root_element,average/count,count,temp);
    xmlFreeDoc (parsedoc);
                

    printf("------------------------\n");
    }

void xml_xsd_validation(){
    //XML oluşturulduktan sonra XSD ile validation gerçekleştirildi.
    xmlDocPtr doc;
	xmlSchemaPtr schema = NULL;
	xmlSchemaParserCtxtPtr ctxt;
	char *XMLFileName = "xml-xsd.xml";
	char *XSDFileName = "xml-xsd.xsd";
	xmlLineNumbersDefault(1);
	ctxt = xmlSchemaNewParserCtxt(XSDFileName);
	schema = xmlSchemaParse(ctxt);
	xmlSchemaFreeParserCtxt(ctxt);
	doc = xmlReadFile(XMLFileName, NULL, 0);
	if (doc == NULL)
	{
		fprintf(stderr, "Could not parse %s\n", XMLFileName);
	}
	else
	{
		xmlSchemaValidCtxtPtr ctxt;
		int ret;
		ctxt = xmlSchemaNewValidCtxt(schema);
		ret = xmlSchemaValidateDoc(ctxt, doc);
        printf("-------------------------\n");
		if (ret == 0)
		{
			printf("#--> %s validates %s <--#\n", XMLFileName,XSDFileName);
		}
		else if (ret > 0)
		{
			printf("####--> %s fails to validate <--####\n", XMLFileName);
		}
		else
		{
			printf("####--> %s validation generated an internal error  <--####\n", XMLFileName);
		}
        printf("-------------------------\n");
		xmlSchemaFreeValidCtxt(ctxt);
		xmlFreeDoc(doc);
	}
	// free the resource
	if(schema != NULL)
		xmlSchemaFree(schema);
	xmlSchemaCleanupTypes();
	xmlCleanupParser();
	xmlMemoryDump();
}