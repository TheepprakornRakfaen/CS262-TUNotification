# INTRODUCING -- TUNOTIFICATION
(โฟค - เกี่ยวกับโปรเจค + Painpoint)

## Solution + AWS Architecture
(โฟค - เขียน Solution + แปะภาพ AWS Architecture พร้อมอธิบายรายละเอียดในภาพ)

# ขั้นตอนการติดตั้ง
  การติดตั้งระบบ TUNotification มีการใช้งานอุปกรณ์ IoT และบริการของ AWS โดยมีขั้นตอนดังนี้
## SECTION 1 : 
### IoT Device / Iot Core
(นาย - เขียนขั้นตอนการประกอบ IoT และการเชื่อมต่อด้วย IoT Core)
### AWS Kinesis data stream / Lambda function
(แม็ค - เขียนขั้นตอนการสร้าง kinesis และ lambda function ฝั่งข้อมูลขาเข้า)
เมื่อสร้าง ทำการ Add trigger แล้วให้ทำขั้นตอนต่อไปนี้
1.เขียนโค้ดดังนี้ลงใน lambda function

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


2.ไปที่เมนู Configuration ในหน้า Lambda

3.เลือกที่ชื่อว่า Permissions

4.ส่วนของ Execution role, ให้กดปุ่ม Edit

5.แล้วกด Save



## SECTION 2 : DATABASE
### DynamoDB
ขั้นตอนเเรก ตั้งชื่อตาราง และกำหนด Partition Key / Sort Key
- ตั้งชื่อตาราง TUNotification ให้ตารางมีเอกลักษณ์ สะดวกในการอ้างอิงจาก Lambda และ API
- 



## SECTION 3 : WEB APPLICATION
### Amplify 
(ต้นน้ำ - เขียนขั้นตอนการสร้าง Amplify และขึ้นหน้าเว็ป)
### MAP API
(โปรแกรม - เขียนขั้นตอนการดึง API แผนที่มาใส่หน้าเว็ป)



### API Gateway / Lambda function
(มิว - เขียนขั้นตอนการสร้าง API Gateway และ lambda function ) 
ขั้นตอนที่ 1: สร้าง IAM Role สำหรับ Lambda

ไปที่ AWS Console → ค้นหา IAM
คลิก Roles → Create role
เลือก AWS service → เลือก Lambda → คลิก Next
ค้นหา Policy ชื่อ AmazonDynamoDBReadOnlyAccess → ติ๊กเลือก → คลิก Next
ตั้งชื่อ Role Name ว่า lambda-dynamodb-role → คลิก Create role

ขั้นตอนที่ 2: สร้าง Lambda Function

ไปที่ AWS Console → ค้นหา Lambda
คลิก Create function
เลือก Author from scratch
กรอกข้อมูลดังนี้

Function name: getSoundData
Runtime: Python 3.12
Architecture: x86_64
Permissions: เลือก Use an existing role → เลือก lambda-dynamodb-role

คลิก Create function

ขั้นตอนที่ 3: เขียน Code ใน Lambda Function

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

หลังวาง Code แล้วคลิก Deploy (ปุ่มสีส้ม)

ขั้นตอนที่ 4: ทดสอบ Lambda Function

คลิกแท็บ Test
คลิก Create new test event → ตั้งชื่อ event ว่า test1 → ใส่ body เป็น {} → คลิก Save
คลิก Test → ตรวจสอบว่า statusCode เป็น 200 และมีข้อมูลส่งกลับมา

ขั้นตอนที่ 5: สร้าง API Gateway (HTTP API)

ไปที่ AWS Console → ค้นหา API Gateway
คลิก Create API
เลือก HTTP API → คลิก Build
กรอก API name ว่า sound-monitoring-api
คลิก Next → Next → Create

ขั้นตอนที่ 6: สร้าง Route และเชื่อมกับ Lambda

คลิกเมนู Routes ด้านซ้าย → คลิก Create
กรอกข้อมูลดังนี้

Method: GET
Path: /sound

คลิก Create
คลิกที่ Route GET /sound → คลิก Attach integration
คลิก Create and attach an integration → เลือก Lambda function
เลือก Lambda Function ชื่อ getSoundData → คลิก Create

ขั้นตอนที่ 7: ตั้งค่า CORS
จำเป็นต้องเปิด CORS เพื่อให้ Amazon Amplify เรียก API ได้

คลิกเมนู CORS ด้านซ้าย → คลิก Configure
กรอกข้อมูลดังนี้

Access-Control-Allow-Origin: *****
Access-Control-Allow-Headers: *****
Access-Control-Allow-Methods: GET, OPTIONS

คลิก Save


ขั้นตอนที่ 8: Deploy API

คลิกเมนู Deploy ด้านซ้าย
เลือก Stage เป็น $default
คลิก Deploy
คัดลอก Invoke URL ที่ได้ เช่น
https://xxxx.execute-api.ap-southeast-1.amazonaws.com

ขั้นตอนที่ 9: ทดสอบ API
เปิด Browser แล้วเข้า URL ดังนี้
GET https://xxxx.execute-api.ap-southeast-1.amazonaws.com/sound


