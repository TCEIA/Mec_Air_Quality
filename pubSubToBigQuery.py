from google.cloud import pubsub_v1
from google.cloud import bigquery
import json

project_id = "ensayo-bme"  # replace with your project id
subscription_name = "storage" # replace with your subscription name

subscriber = pubsub_v1.SubscriberClient()
subscription_path = subscriber.subscription_path(
    project_id, subscription_name)



client = bigquery.Client(project=project_id)
dataset_id = 'sensors_data' # replace with your dataset ID
table_id = 'temp_gps1'  # replace with your table ID
table_ref = client.dataset(dataset_id).table(table_id)
table = client.get_table(table_ref)  # API request
print(table)

def callback(message):
    try:
        dict1 = eval(message.data.decode())
        print(dict1)
        print(type(dict1))
        errors = client.insert_rows(table, [dict1])
        print(errors)
        #assert errors == []
        if(float(dict1['Temperature'])<100):
            message.ack()
    except AssertionError:
        print('error inserting message with message id: ', errors)
    except Exception as e:
        print('error decoding message on {}' .format(e))

future = subscriber.subscribe(subscription_path, callback=callback)

try:
    # When timeout is unspecified, the result method waits indefinitely.
    future.result(timeout=60)
except Exception as e:
    print('Listening for messages on {} threw an Exception: {}.'.format(subscription_name, e))
