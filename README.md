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
(โอม - เขียนขั้นตอนการสร้าง Dynamodb)

## SECTION 3 : WEB APPLICATION
### Amplify 
(ต้นน้ำ - เขียนขั้นตอนการสร้าง Amplify และขึ้นหน้าเว็ป)
### MAP API
(โปรแกรม - เขียนขั้นตอนการดึง API แผนที่มาใส่หน้าเว็ป)
### API Gateway / Lambda function
(โปรแกรม - เขียนขั้นตอนการสร้าง kinesis และ lambda function ฝั่งข้อมูลขาเข้า) 
