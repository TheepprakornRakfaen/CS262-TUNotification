# INTRODUCING -- TUNOTIFICATION
(โฟค - เกี่ยวกับโปรเจค + Painpoint)

## Solution + AWS Architecture
(โฟค - เขียน Solution + แปะภาพ AWS Architecture พร้อมอธิบายรายละเอียดในภาพ)
<br><br>
# ขั้นตอนการติดตั้ง
การติดตั้งระบบ TUNotification มีการใช้งานอุปกรณ์ IoT และบริการของ AWS โดยมีขั้นตอนดังนี้
## SECTION 1 : AWS Kinesis data stream/Iot device/Lambda function
### AWS Kinesis data stream : เตรียมสร้างท่อส่งข้อมูล
1.ค้นหา **Kinesis** ใน Search bar <br>
2.กด **Create data stream** <br>
3.ตั้งชื่อ Data stream name ว่า **SoundDataStream** <br>
4.เลือก Capacity Mode เป็น **Provisioned** <br>
5.เลือกจำนวน **Provisioned Shards** เป็น 1 <br>
6.กด **Create data stream** 

---
### IoT Device / Iot Core : เชื่อมต่อและส่งข้อมูลด้วย IoT Core




---
### lambda function : ส่งข้อมูลไปยัง Database ด้วย Lambda function
1.เขียนโค้ดดังนี้ลงใน lambda function
```Python
    const { DynamoDBClient } = require("@aws-sdk/client-dynamodb");
    const { DynamoDBDocumentClient, PutCommand } = require("@aws-sdk/lib-dynamodb");
    
    const client = new DynamoDBClient({});
    const docClient = DynamoDBDocumentClient.from(client);
      exports.handler = async (event) => {
      console.log("Receiving data from Kinesis...");
        for (const record of event.Records) {
            try {
                // 1. Decode ข้อมูลจาก Base64 เป็น String และแปลงเป็น JSON
                const payload = Buffer.from(record.kinesis.data, 'base64').toString('ascii');
                const data = JSON.parse(payload);
              
              console.log("Decoded Data:", data);
           
              const params = {
                  TableName: "Your_DynamoDB_Table_Name", // เปลี่ยนเป็นชื่อตารางของคุณ
                  Item: {
                      "location_name": data.location_name, // Partition Key
                      "timestamp": data.timestamp || new Date().toISOString(), // Sort Key (ถ้ามี)
                      "coordinates": data.coordinates,
                      "noise_level": data.noise_level,
                      "raw_data": data
                  }
              };
              
              await docClient.send(new PutCommand(params));
              console.log("Successfully saved to DynamoDB");
  
          } catch (error) {
              console.error("Error processing record:", error);
          }
      }
      
      return { status: "success" };
    };"
```

2.ไปที่เมนู Configuration ในหน้า Lambda <br>
3.เลือกที่ชื่อว่า Permissions <br>
4.ส่วนของ Execution role, ให้กดปุ่ม Edit <br>
5.แล้วกด Save

<br><br>
## SECTION 2 : DATABASE
### DynamoDB
1. ตั้งชื่อตาราง และกำหนด Partition Key / Sort Key
- ตั้งชื่อตาราง TUNotification ให้ตารางมีเอกลักษณ์ สะดวกในการอ้างอิงจาก Lambda และ API
- Partition Key = DeviceID กระจายข้อมูลไปยังเซิร์ฟเวอร์หลายๆ ตัว (Sharding) ทำให้ระบบรองรับข้อมูลปริมาณมากได้ และค้นหาตามสถานที่ได้เร็ว
- Sort Key = timestamp เรียงลำดับข้อมูลตามเวลาได้ทันที รองรับการ Query ช่วงเวลา (เช่น "ดูข้อมูลตึกA ช่วง 9:00-10:00")

2. เลือก On-demand Capacity
- **On-demand** : ไม่ต้องคาดเดาปริมาณการใช้งานล่วงหน้า ระบบจะปรับขนาดอัตโนมัติตามปริมาณข้อมูลที่เข้ามา
Encryption
- **AWS owned key** : เข้ารหัสข้อมูลที่เก็บในฐานข้อมูลโดยอัตโนมัติ ป้องกันข้อมูลรั่วไหลหากมีผู้ไม่ได้รับอนุญาตเข้าถึง
**Attributes** อื่นๆ
- ในขั้นตอนนี้เมื่อรับข้องมูลจาก IOT เข้ามาจะทำการสร้าง Attributes อัตโนมัติ อย่าง
- - coordinates	เก็บพิกัด GPS เพื่อแสดงบนแผนที่ในเว็บ
  - noise_level	ค่าระดับเสียงที่วัดได้ ใช้เป็นข้อมูลหลักในการแจ้งเตือน
  - raw_data	เก็บข้อมูลดิบทั้งหมดจากเซนเซอร์ กรณีต้องการตรวจสอบย้อนหลังหรือ Debug
  
**สรุปจุดประสงค์หลักของ DynamoDB**
- เก็บข้อมูล Real-time	รับข้อมูลจากเซนเซอร์ที่ส่งมาตลอดเวลา
- Query เร็ว	ค้นหาข้อมูลตามสถานที่และช่วงเวลาได้ทันที
- รองรับการขยาย	ข้อมูลเพิ่มเป็นล้านแถว ระบบก็ยังทำงานได้ปกติ
- เชื่อมต่อกับ API	ให้หน้าเว็บดึงข้อมูลไปแสดงผลได้

<br><br>



## SECTION 3 : WEB APPLICATION

### Lambda function : สร้าง Lambda funcion สำหรับดึงข้อมูล
1. ไปที่ AWS Console → ค้นหา Lambda <br>
2. คลิก Create function <br>
3. Author from scratch <br>
4. กรอกข้อมูลดังนี้
 - Function name: getSoundData
 - Runtime: Python 3.12
 - Architecture: x86_64
 - Permissions: เลือก Use an existing role → เลือก Labrole
5. คลิก Create function <br>
6. นำโค้ดต่อไปนี้วางลงใน function

```Python
import json
import boto3
from decimal import Decimal

dynamodb = boto3.resource('dynamodb', region_name='us-east-1')  
table = dynamodb.Table('TUNotification')  

def decimal_default(obj):
    if isinstance(obj, Decimal):
        return float(obj)
    raise TypeError

def lambda_handler(event, context):
    try:
        response = table.scan()
        items = response.get('Items', [])
        
        return {
            'statusCode': 200,
            'headers': {
                'Access-Control-Allow-Origin': '*',
                'Content-Type': 'application/json'
            },
            'body': json.dumps(items, default=decimal_default)
        }
    except Exception as e:
        return {
            'statusCode': 500,
            'headers': {'Access-Control-Allow-Origin': '*'},
            'body': json.dumps({'error': str(e)})
        }
```
7.หลังวาง Code แล้วคลิก Deploy (ปุ่มสีส้ม) <br>

---
### API Gateway : API สำหรับดึงข้อมูลแสดงบนหน้าเว็ป
1. AWS Console → ค้นหา **API Gateway**  <br>
2. คลิก **Create API**  <br>
3. **HTTP API** → คลิก **Build**  <br>
4. API name ว่า **sound-monitoring-api**  <br>
5. **Next** → **Next** → **Create**  <br>
6. สร้าง Route และเชื่อมกับ **Lambda**  <br>
7. Routes ด้านซ้าย → คลิก Create กรอกข้อมูลดังนี้ <br>
- Method: GET
- Path: /sound
8. Create <br>
9. Route GET /sound → คลิก **Attach integration** <br>
10. **Create and attach an integration** → เลือก **Lambda function** <br>
11. Lambda Function ชื่อ **getSoundData** → คลิก Create <br>
12. CORS ด้านซ้าย → คลิก **Configure** กรอกข้อมูลดังนี้ <br>
- Access-Control-Allow-Origin: ***** <br>
- Access-Control-Allow-Headers: *****
- Access-Control-Allow-Methods: GET, OPTIONS <br>
13. คลิก **Save** <br>
14. คลิกเมนู **Deploy** ด้านซ้าย เลือก **Stage** เป็น $default <br>
15. คลิก **Deploy** <br>
ผลลัพธ์สามารถคัดลอก URL นั้นคือ https://xxxx.execute-api.ap-southeast-1.amazonaws.com/sound ใส่ใน Web application เพื่อดึงข้อมูลได้ <br>

---
### Amplify 
(ต้นน้ำ - เขียนขั้นตอนการสร้าง Amplify และขึ้นหน้าเว็ป)
### MAP API
(โปรแกรม - เขียนขั้นตอนการดึง API แผนที่มาใส่หน้าเว็ป)

