# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0022_lamp_online_timestamp'),
    ]

    operations = [
        migrations.CreateModel(
            name='Lamp_Errors',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('lamp_number', models.CharField(default=b'None', max_length=256, db_column=b'lamp_number', blank=True)),
                ('error_status', models.CharField(default=b'None', max_length=256, db_column=b'error_status', blank=True)),
            ],
        ),
        migrations.RemoveField(
            model_name='lamp_register_queue',
            name='error_status',
        ),
        migrations.RemoveField(
            model_name='lamp_register_queue',
            name='online_status',
        ),
    ]
