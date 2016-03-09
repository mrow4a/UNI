# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0034_auto_20150831_1456'),
    ]

    operations = [
        migrations.AlterField(
            model_name='lamp_software',
            name='car_detection_soft',
            field=models.FileField(upload_to=b'software/<function now at 0xb6c2402c>'),
        ),
        migrations.AlterField(
            model_name='lamp_software',
            name='id',
            field=models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True),
        ),
    ]
