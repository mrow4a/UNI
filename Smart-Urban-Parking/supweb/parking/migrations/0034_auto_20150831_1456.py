# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0033_auto_20150831_1455'),
    ]

    operations = [
        migrations.AlterField(
            model_name='lamp_software',
            name='car_detection_soft',
            field=models.FileField(upload_to=b'software/<function now at 0xb6c5302c>'),
        ),
    ]
